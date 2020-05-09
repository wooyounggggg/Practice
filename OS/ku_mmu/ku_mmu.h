#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PRESENT 1
#define SWAPPED 0
#define INVALID -1
#define PDE_INDEX 0
#define PMDE_INDEX 1
#define PTE_INDEX 2
#define PAGE_INDEX 3
#define DIRECTORY 3
#define PAGE 2
#define TABLE 1
typedef struct ku_pte
{
    char entry;
} KU_PTE;
typedef struct PCB
{
    char pid;
    struct PCB *next;
    struct PCB *prev;
    struct ku_pte *PDBR;
} PCB;
typedef struct FreeListElement
{
    struct FreeListElement *next;
    struct FreeListElement *prev;
    struct ku_pte *parentPTE;
    char PFN;
} FreeListElement;
PCB *PCBHeader = NULL;
struct ku_pte *pmem = NULL;
struct ku_pte *swapSpace = NULL;        /* Swap Space에는 page만 들어가므로, page를 pointing하는 Parent PTE를 찾아야 page가 어떤 PTE로부터 파생되었는지 구별이 가능하다. */
FreeListElement *freeListHeader = NULL; /* pmem에 Page를 삽입할 때 add되는 공간. PFN number 및 할당된 Page의 부모 PTE를 저장함. swap space에 삽입될 때에는 PFN은 버리고 부모 PTE만 저장 */

FreeListElement *addFreeListElement(struct ku_pte *, char, FreeListElement *, FreeListElement *);
int setFreeListElement(FreeListElement *, struct ku_pte *, char, FreeListElement *, FreeListElement *);
PCB *addPCBElement(char);
int setPCB(PCB *, PCB *, PCB *, char);
PCB *getPCBByPid(char);
int getPTEState(struct ku_pte *);
char *getPageIndexesByVA(char);
char getNotUsingPFN();
int mapDirectory(struct ku_pte *);
int mapTable(struct ku_pte *);
int mapPage(struct ku_pte *);
/* int mapPageOrTable(struct ku_pte *, int); */
int swapPage(struct ku_pte *);
struct ku_pte *getPmemByPFN(char);
char getEntryByPFN(char);
char getPFNByEntry(char);
int mappingProcess(struct ku_pte *, char);
int ku_page_fault(char, char);
void *ku_mmu_init(unsigned int, unsigned int);
int ku_run_proc(char, struct ku_pte **);

/* handle page fault by 'demand page' or 'swapping(FIFO)'. Page Directory and Page Table not swapped out */
void *ku_mmu_init(unsigned int mem_size, unsigned int swap_size) /* initialize resource. called only once in starting. */
{
    /*1. Allocate memory space for pmem */
    pmem = (struct ku_pte *)malloc(sizeof(struct ku_pte) * mem_size);
    // struct ku_pte *pmem = (struct ku_pte *)malloc(sizeof(struct ku_pte) * mem_size);
    if (pmem == NULL)
        return 0;
    /*2. Allocate swap space*/
    swapSpace = (struct ku_pte *)malloc(sizeof(struct ku_pte) * swap_size);
    if (swapSpace == NULL)
    {
        free(pmem);
        return 0;
    }
    return pmem;
}
int ku_run_proc(char pid, struct ku_pte **ku_cr3) /* Performs Context Switch. If pid is new, function creates a process in virtual and its page directory */
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
int mappingProcess(struct ku_pte *pageDirectory, char va) /* map Page Directory ~ Page to pmem */
{
    struct ku_pte *selectedPTE;
    char *pageIndexes = getPageIndexesByVA(va);
    if (pageIndexes == NULL)
        return 0;
    selectedPTE = pageDirectory + pageIndexes[PDE_INDEX]; /* Search Page Directory Entry */
    /* page directory processing : selectedPTE = Page Directory */
    if (getPTEState(selectedPTE) == INVALID) /* if searched PTE is INVALID, */
        mapTable(selectedPTE);               /* map PTE referenced by selectedPTE */
    /* else if (getPTEState(selectedPTE) == SWAPPED)
        swapPage(selectedPTE); */
    selectedPTE = getPmemByPFN(getPFNByEntry(selectedPTE->entry)) + /* Search Page Middle Directory entry */
                  pageIndexes[PMDE_INDEX];
    /* page middle directory processing : selectedPTE = Page Middle Directory*/
    if (getPTEState(selectedPTE) == INVALID) /* if searched PTE is INVALID, */
        mapTable(selectedPTE);               /* map PTE referenced by selectedPTE */
    /* else if (getPTEState(selectedPTE) == SWAPPED)
        swapPage(selectedPTE); */
    selectedPTE = getPmemByPFN(getPFNByEntry(selectedPTE->entry)) + /* Search Page Directory entry */
                  pageIndexes[PTE_INDEX];
    /* page table processing : selectedPTE = Page Table*/
    if (getPTEState(selectedPTE) == INVALID) /* if searched PTE is INVALID, */
        mapPage(selectedPTE);                /* map page referenced by selectedPTE */
    else if (getPTEState(selectedPTE) == SWAPPED)
        swapPage(selectedPTE);
    free(pageIndexes);
    return 1;
}
char getNotUsingPFN() /* Iterating pmem, find default state page. If no page of default, return FreeList's header PFN and move header to next */
{
    char PFN = 1;
    struct ku_pte *tmpPmem = getPmemByPFN(PFN);
    while (tmpPmem != NULL) /* iterate pmem in first (for default pmem) */
    {
        if (tmpPmem->entry & 0x02)
            return PFN;
        PFN++;
        tmpPmem = getPmemByPFN(PFN);
    }
    PFN = freeListHeader->PFN;
    freeListHeader = freeListHeader->next;
    free(freeListHeader->prev);
    return PFN;
}
/* Page Directory와 Page Middle Dir, Page Table 및 Page를 pmem에 적재해야 하는데, Page Directory는 PCB->PDBR로 적재를 하지 va를 통한 indexing으로 적재하지 않기 때문에,
이를 고려하여서 분기로 적재해주거나 따로 함수를 하나 만들어야 함. addPCBElement에 Page Directory를 대상으로 mapPageOrTable을 수행하기는 했으나, 논리가 완성되지 않음. */
int mapDirectory(struct ku_pte *PDBR)
{
    /* 1. Get PFN of being not used space or FIFO-page(getNoUsingPFN function) */

    /* 2. Get Entry of PFN found in seq 1*/
    /* 3. Allocate parentPTE to pmem with no-use-PFN (pmem's address = PDBR)
        # To get directory from pmem, function getDirectoryByAddress(ku_pte *ku_cr3) is needed.
    */
}
int mapTable(struct ku_pte *parentPTE)
{
    /* 1. Get PFN of being not used space or FIFO-page(getNoUsingPFN function) */

    /* 2. Get Entry of PFN found in seq 1*/
    /* 
        3. Allocate that entry to parent PTE
        4. Make new table : char[4]
        5. Allocate new table's address to pmem with no-use-PFN (pmem's address = )
    */
}
int mapPage(struct ku_pte *parentPTE)
{
    /* 1. Get PFN of being not used space or FIFO-page(getNoUsingPFN function) */

    /* 2. Get Entry of PFN found in seq 1*/
}
int swapPage(struct ku_pte *parentPTE)
{
}
struct ku_pte *getPmemByPFN(char PFN)
{
    return pmem + PFN * 4;
}
char getEntryByPFN(char PFN)
{
    return PFN << 2 + 1;
}
char getPFNByEntry(char entry)
{
    return entry >> 2;
}
int setFreeListElement(FreeListElement *newFreeListElement, struct ku_pte *parentPTE, char PFN, FreeListElement *next, FreeListElement *prev)
{
    if (newFreeListElement == NULL || parentPTE == NULL)
        return 0;
    newFreeListElement->PFN = PFN;
    newFreeListElement->parentPTE = parentPTE;
    newFreeListElement->next = next;
    newFreeListElement->prev = prev;
    return 1;
}
FreeListElement *addFreeListElement(struct ku_pte *parentPTE, char PFN, FreeListElement *next, FreeListElement *prev)
{
    FreeListElement *newFreeListElement = freeListHeader;
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
    newPCB->PDBR = (struct ku_pte *)malloc(sizeof(struct ku_pte) * 4);
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
int getPTEState(struct ku_pte *PTE)
{
    if (PTE->entry == 0)
        return INVALID;
    else if ((PTE->entry & 0x01) && (PTE->entry >> 2)) /* if Present bit is 1 and PFN > 0, return PRESENT */
        return PRESENT;
    else if (!(PTE->entry & 0x01) && (PTE->entry >> 1)) /* if Present bit is 0 and Swap Offset > 0, return SWAPPED */
        return SWAPPED;
    else if (PTE->entry & 0x02)
        return PAGE;
}
char *getPageIndexesByVA(char va)
{
    char *pageIndexes;
    pageIndexes = (char *)malloc(sizeof(char) * 4);
    if (pageIndexes == NULL)
        return NULL;
    pageIndexes[PDE_INDEX] = va >> 6;
    pageIndexes[PMDE_INDEX] = va >> 4 & 0x03;
    pageIndexes[PTE_INDEX] = va >> 2 & 0x03;
    pageIndexes[PAGE_INDEX] = va & 0x03;
    return pageIndexes;
}