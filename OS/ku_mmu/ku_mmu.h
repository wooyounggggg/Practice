#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* invalid test */
#define PRESENT 1
#define SWAPPED 0
#define INVALID -1
/* page index */
#define PDE_INDEX 0
#define PMDE_INDEX 1
#define PTE_INDEX 2
#define PAGE_INDEX 3
/* page flag */
#define PAGE 2
#define TABLE 1
/* page offset */
#define PAGE_OFFSET 4
/* not using pfn location */
#define IN_PMEM 0
#define IN_FREE_LIST 1
typedef struct ku_pte
{
    char entry;
} KU_PTE;
typedef struct PCB
{
    char pid;
    struct PCB *next;
    struct PCB *prev;
    KU_PTE *PDBR;
} PCB;
typedef struct FreeListElement
{
    struct FreeListElement *next;
    struct FreeListElement *prev;
    KU_PTE *parentPTE;
    char PFN;
} FreeListElement;
PCB *PCBHeader = NULL;
KU_PTE *pmem = NULL;
KU_PTE *swapSpace = NULL;               /* Swap Space에는 page만 들어가므로, page를 pointing하는 Parent PTE를 찾아야 page가 어떤 PTE로부터 파생되었는지 구별이 가능하다. */
FreeListElement *freeListHeader = NULL; /* pmem에 Page를 삽입할 때 add되는 공간. PFN number 및 할당된 Page의 부모 PTE를 저장함. swap space에 삽입될 때에는 PFN은 버리고 부모 PTE만 저장 */

void initializeSwapSpace(unsigned int);
void initializePmem(unsigned int);
void initializeTable(KU_PTE *);
FreeListElement *getTrailerOfFreeList();
FreeListElement *addFreeListElement(KU_PTE *, char, FreeListElement *, FreeListElement *);
int setFreeListElement(FreeListElement *, KU_PTE *, char, FreeListElement *, FreeListElement *);
PCB *addPCBElement(char);
int setPCB(PCB *, PCB *, PCB *, char);
PCB *getPCBByPid(char);
int getPTEState(KU_PTE *);
char *getPageIndexesByVA(char);
int getNotUsingPFN(char *);
void setDirToPmem(KU_PTE *, KU_PTE *);
int mapDirectory(KU_PTE *);
void setTableToPmem(KU_PTE *, KU_PTE *);
int mapTable(KU_PTE *);
int mapPage(KU_PTE *);
void setPageToPmem(KU_PTE *, KU_PTE *);
int swapPage(KU_PTE *);
void setTableEntryWithIndex(KU_PTE *, char, char);
void setTableEntry(KU_PTE *, char);
KU_PTE *getPageOrTableByPFN(char);
char getEntryByPFN(char);
char getPFNByEntry(char);
int mappingProcess(KU_PTE *, char);
int ku_page_fault(char, char);
void *ku_mmu_init(unsigned int, unsigned int);
int ku_run_proc(char, KU_PTE **);

/* handle page fault by 'demand page' or 'swapping(FIFO)'. Page Directory and Page Table not swapped out */
void *ku_mmu_init(unsigned int mem_size, unsigned int swap_size) /* initialize resource. called only once in starting. */
{
    /*1. Allocate memory space for pmem */
    pmem = (KU_PTE *)malloc(sizeof(KU_PTE) * mem_size);
    if (pmem == NULL)
        return 0;
    initializePmem(mem_size);
    /*2. Allocate swap space*/
    swapSpace = (KU_PTE *)malloc(sizeof(KU_PTE) * swap_size);
    if (swapSpace == NULL)
    {
        free(pmem);
        return 0;
    }
    initializeSwapSpace(swap_size);
    return pmem;
}
int ku_run_proc(char pid, KU_PTE **ku_cr3) /* Performs Context Switch. If pid is new, function creates a process in virtual and its page directory */
{
    /* 1. get PCB(global) which has parameter local variable pid(functionally) : getPCBByPid(char pid) */
    PCB *PCBByPid = getPCBByPid(pid);
    if (PCBByPid == NULL)
        if ((PCBByPid = addPCBElement(pid)) == NULL)
            return -1;
    /* 2. map ku_cr3 to PCB's PDBR */
    *ku_cr3 = PCBByPid->PDBR;
    return 0;
}
int ku_page_fault(char pid, char va)
{
    /* 1. get PCB by PID. */
    PCB *PCBByPid = getPCBByPid(pid);
    if (PCBByPid == NULL) /* if NULL, ku_run_proc err */
        return -1;
    /* 2. Page Directory ~ Page Mapping*/
    if (mappingProcess(PCBByPid->PDBR, va))
        return 0;
    return -1; /* else return -1 */
}
void initializeSwapSpace(unsigned int swap_size)
{
    KU_PTE *tmp = swapSpace;
    for (int i = 0; i < swap_size; i += PAGE_OFFSET)
    {
        if (i != 0)
            tmp->entry = 0x03;
        tmp += PAGE_OFFSET;
    }
}
void initializePmem(unsigned int mem_size)
{
    KU_PTE *tmp = pmem;
    for (int i = 0; i < mem_size; i += PAGE_OFFSET)
    {
        if (i != 0)
            tmp->entry = 0x03;
        tmp += PAGE_OFFSET;
    }
}
void initializeTable(KU_PTE *pte)
{
    for (int i = 0; i < PAGE_OFFSET; i++)
    {
        pte->entry = 0x00;
        pte++;
    }
}
int mappingProcess(KU_PTE *pageDirectory, char va) /* map Page Directory ~ Page to pmem */
{
    KU_PTE *selectedPTE;
    char *pageIndexes = getPageIndexesByVA(va);
    printf("pageIndexes : %d %d %d %d\n", pageIndexes[0], pageIndexes[1], pageIndexes[2], pageIndexes[3]);
    if (pageIndexes == NULL)
        return 0;
    printf("mapping Process start\n");
    printf("page directory first entry invalid Test : %d\n", getPTEState(pageDirectory));
    selectedPTE = pageDirectory + pageIndexes[PDE_INDEX]; /* Search Page Directory Entry */
    printf("page directory indexed entry invalid Test(before map) : %d\n", getPTEState(selectedPTE));
    /* page directory processing : selectedPTE = Page Directory */
    if (getPTEState(selectedPTE) == INVALID) /* if searched PTE is INVALID, */
        mapTable(selectedPTE);               /* map PTE referenced by selectedPTE */
    printf("page directory indexed entry invalid Test(after map) : %d\n", getPTEState(selectedPTE));
    selectedPTE = getPageOrTableByPFN(getPFNByEntry(selectedPTE->entry)) + /* Search Page Middle Directory entry */
                  pageIndexes[PMDE_INDEX];
    printf("page middld directory entry invalid Test(before map) : %d\n", getPTEState(selectedPTE));
    /* page middle directory processing : selectedPTE = Page Middle Directory*/
    if (getPTEState(selectedPTE) == INVALID) /* if searched PTE is INVALID, */
        mapTable(selectedPTE);               /* map PTE referenced by selectedPTE */
    printf("page middld directory entry invalid Test(after map) : %d\n", getPTEState(selectedPTE));
    selectedPTE = getPageOrTableByPFN(getPFNByEntry(selectedPTE->entry)) + /* Search Page Directory entry */
                  pageIndexes[PTE_INDEX];
    /* page table processing : selectedPTE = Page Table*/
    printf("page table entry invalid Test(before map) : %d\n", getPTEState(selectedPTE));
    if (getPTEState(selectedPTE) == INVALID) /* if searched PTE is INVALID, */
        mapPage(selectedPTE);                /* map page referenced by selectedPTE */
    else if (getPTEState(selectedPTE) == SWAPPED)
        swapPage(selectedPTE);
    printf("page table entry invalid Test(after map) : %d\n", getPTEState(selectedPTE));
    free(pageIndexes);
    printf("mapping Process end\n");
    return 1;
}
int getNotUsingPFN(char *notUsingPFN) /* Iterating pmem, find default state page. If no page of default, return FreeList's header PFN and move header to next */
{
    char PFN = 1;
    KU_PTE *tmpPmem = getPageOrTableByPFN(PFN);
    while (tmpPmem != NULL) /* iterate pmem in first (for default pmem) */
    {
        if (tmpPmem->entry & 0x03)
        {
            *notUsingPFN = PFN;
            return IN_PMEM;
        }
        PFN++;
        tmpPmem = getPageOrTableByPFN(PFN);
    }
    PFN = freeListHeader->PFN;
    freeListHeader = freeListHeader->next;
    free(freeListHeader->prev);
    *notUsingPFN = PFN;
    return IN_FREE_LIST;
}
/* Page Directory와 Page Middle Dir, Page Table 및 Page를 pmem에 적재해야 하는데, Page Directory는 PCB->PDBR로 적재를 하지 va를 통한 indexing으로 적재하지 않기 때문에,
이를 고려하여서 분기로 적재해주거나 따로 함수를 하나 만들어야 함. addPCBElement에 Page Directory를 대상으로 mapPageOrTable을 수행하기는 했으나, 논리가 완성되지 않음. */
void setDirToPmem(KU_PTE *notUsingPmem, KU_PTE *PDBR)
{
    for (int i = 0; i < PAGE_OFFSET; i++)
        (notUsingPmem + i)->entry = (PDBR + i)->entry;
}
int mapDirectory(KU_PTE *PDBR)
{
    /* Directory에 대한 정보는 PCB에서 갖고 있기 때문에, PFN을 통해 Dir에 접근할 필요 없음 */
    /* 1. Get PFN of being not used space or FIFO-page(getNoUsingPFN function) */
    char notUsingPFN;
    int notUsingPFNLocation = getNotUsingPFN(&notUsingPFN);
    printf("Not Using PFN : %d\n", notUsingPFN);
    /* 2. Allocate Directory to pmem with no-use-PFN (pmem's entry = PDBR)*/
    KU_PTE *notUsingPmem = getPageOrTableByPFN(notUsingPFN);
    initializeTable(PDBR);
    if (notUsingPFNLocation == IN_PMEM)
        setDirToPmem(notUsingPmem, PDBR);
    else
    {
        swapPage(notUsingPmem);
        setDirToPmem(notUsingPmem, PDBR);
    }
    printf("mapDirectory\n");
    return 1;
}
void setTableToPmem(KU_PTE *notUsingPmem, KU_PTE *table)
{
    for (int i = 0; i < PAGE_OFFSET; i++)
        (notUsingPmem + i)->entry = (table + i)->entry;
}
int mapTable(KU_PTE *parentPTE)
{
    /* 1. Get PFN of being not used space or FIFO-page(getNoUsingPFN function) */
    char notUsingPFN;
    int notUsingPFNLocation = getNotUsingPFN(&notUsingPFN);
    KU_PTE *notUsingPmem = getPageOrTableByPFN(notUsingPFN);
    /* 2. Get Entry of PFN found in seq 1*/
    char newEntry = getEntryByPFN(notUsingPFN);
    /* test */
    printf("Not Using PFN : %d\n", notUsingPFN);
    /* test */
    /* 3. Allocate that entry to parent's PTE*/
    setTableEntry(parentPTE, newEntry);
    /* 4. Make new table : KU_PTE[4]*/
    KU_PTE *newTable = (KU_PTE *)malloc(sizeof(KU_PTE) * PAGE_OFFSET);
    /* 5. Initialize new Table */
    initializeTable(newTable);
    /* 6. if 'not using location' is in pmem, just allocate new table to pmem*/
    if (notUsingPFNLocation == IN_PMEM)
        setTableToPmem(notUsingPmem, newTable);
    /* 7. if 'not using location' is in free-list, swap it to swapSpace and allocate new table to pmem */
    else
    {
        swapPage(notUsingPmem);
        setTableToPmem(notUsingPmem, newTable);
    }
    printf("mapTable\n");
    return 1;
}
int mapPage(KU_PTE *parentPTE)
{
    /* 1. Get PFN of being not used space or FIFO-page(getNoUsingPFN function) */
    char notUsingPFN;
    int notUsingPFNLocation = getNotUsingPFN(&notUsingPFN);
    KU_PTE *notUsingPmem = getPageOrTableByPFN(notUsingPFN);
    /* 2. Get Entry of PFN found in seq 1*/
    char newEntry = getEntryByPFN(notUsingPFN);
    /* 3. Allocate that entry to parent's PTE*/
    setTableEntry(parentPTE, newEntry);
    /* 4. Make new page : KU_PTE[4]*/
    KU_PTE *newPage = (KU_PTE *)malloc(sizeof(KU_PTE) * PAGE_OFFSET);
    /* 5. Initialize new Table */
    initializeTable(newPage);
    /* 6. if 'not using location' is in pmem, allocate new table to pmem and add Free-list element with PFN*/
    if (notUsingPFNLocation == IN_PMEM)
    {
        setPageToPmem(notUsingPmem, newPage);
        addFreeListElement(newPage, notUsingPFN, NULL, getTrailerOfFreeList());
    }
    /* 7. if 'not using location' is in free-list, swap it to swapSpace and allocate new table to pmem and Free-list element with PFN */
    else
    {
        swapPage(notUsingPmem);
        setPageToPmem(notUsingPmem, newPage);
        addFreeListElement(newPage, notUsingPFN, NULL, getTrailerOfFreeList());
    }
    printf("mapPage\n");
    return 1;
}
void setPageToPmem(KU_PTE *notUsingPmem, KU_PTE *page)
{
    for (int i = 0; i < PAGE_OFFSET; i++)
        (notUsingPmem + i)->entry = (page + i)->entry;
}
int swapPage(KU_PTE *parentPTE)
{
}
void setTableEntry(KU_PTE *pte, char entry)
{
    pte->entry = entry;
}
void setTableEntryWithIndex(KU_PTE *table, char idx, char entry)
{
    (table + idx)->entry = entry;
}
KU_PTE *getPageOrTableByPFN(char PFN)
{
    return pmem + PFN * PAGE_OFFSET;
}
char getEntryByPFN(char PFN)
{
    return (PFN << 2) + 1;
}
char getPFNByEntry(char entry)
{
    return entry >> 2;
}
int setFreeListElement(FreeListElement *newFreeListElement, KU_PTE *parentPTE, char PFN, FreeListElement *next, FreeListElement *prev)
{
    if (newFreeListElement == NULL || parentPTE == NULL)
        return 0;
    newFreeListElement->PFN = PFN;
    newFreeListElement->parentPTE = parentPTE;
    newFreeListElement->next = next;
    newFreeListElement->prev = prev;
    return 1;
}
FreeListElement *getTrailerOfFreeList()
{
    FreeListElement *tmp = freeListHeader;
    while (tmp != NULL)
        if (tmp->next == NULL)
            return tmp;
    return tmp;
}
FreeListElement *addFreeListElement(KU_PTE *parentPTE, char PFN, FreeListElement *next, FreeListElement *prev)
{
    FreeListElement *newFreeListElement = freeListHeader;
    int i = 0;
    if (newFreeListElement == NULL)
    {
        freeListHeader = (FreeListElement *)malloc(sizeof(FreeListElement));
        if (freeListHeader == NULL)
            return NULL;
        if (!setFreeListElement(freeListHeader, parentPTE, PFN, NULL, NULL))
        {
            free(freeListHeader);
            return NULL;
        }
        return freeListHeader;
    }
    while (newFreeListElement->next != NULL)
        newFreeListElement = newFreeListElement->next;
    newFreeListElement->next = (FreeListElement *)malloc(sizeof(FreeListElement));
    if (newFreeListElement->next == NULL)
        return NULL;
    if (!setFreeListElement(newFreeListElement->next, parentPTE, PFN, next, prev))
    {
        free(newFreeListElement->next);
        return NULL;
    }
    return newFreeListElement;
}
int setPCB(PCB *newPCB, PCB *next, PCB *prev, char pid)
{
    newPCB->PDBR = (KU_PTE *)malloc(sizeof(KU_PTE) * PAGE_OFFSET);
    if (newPCB->PDBR == NULL)
        return 0;
    newPCB->pid = pid;
    newPCB->next = next;
    newPCB->prev = prev;
    return 1;
}
PCB *addPCBElement(char pid) /* add Process' PCB element and map PDBR to pmem */
{
    PCB *newPCB = PCBHeader;
    if (PCBHeader == NULL)
    {
        PCBHeader = (PCB *)malloc(sizeof(PCB));
        if (PCBHeader == NULL)
            return NULL;
        if (!setPCB(PCBHeader, NULL, NULL, pid))
        {
            free(PCBHeader);
            return NULL;
        }
        mapDirectory(PCBHeader->PDBR);
        return PCBHeader;
    }
    while (newPCB->next != NULL)
        newPCB = newPCB->next;
    newPCB->next = (PCB *)malloc(sizeof(PCB));
    if (newPCB->next == NULL)
        return NULL;
    if (!setPCB(newPCB->next, NULL, newPCB, pid))
    {
        free(newPCB->next);
        return NULL;
    }
    mapDirectory(newPCB->PDBR);
    return newPCB;
}
PCB *getPCBByPid(char pid)
{
    PCB *tmp = PCBHeader;
    while (tmp != NULL)
    {
        if (tmp->pid == pid)
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
}
int getPTEState(KU_PTE *PTE)
{
    if (PTE->entry == 0)
        return INVALID;
    else if ((PTE->entry & 0x01) && (PTE->entry >> 2)) /* if Present bit is 1 and PFN > 0, return PRESENT */
        return PRESENT;
    else if (!(PTE->entry & 0x01) && (PTE->entry >> 1)) /* if Present bit is 0 and Swap Offset > 0, return SWAPPED */
        return SWAPPED;
    else if (PTE->entry & 0x03)
        return PAGE; /* 0000 0011 for default page */
}
char *getPageIndexesByVA(char va)
{
    char *pageIndexes;
    pageIndexes = (char *)malloc(sizeof(char) * 4);
    if (pageIndexes == NULL)
        return NULL;
    pageIndexes[PDE_INDEX] = va >> 6 & 0x03;
    pageIndexes[PMDE_INDEX] = va >> 4 & 0x03;
    pageIndexes[PTE_INDEX] = va >> 2 & 0x03;
    pageIndexes[PAGE_INDEX] = va & 0x03;
    return pageIndexes;
}