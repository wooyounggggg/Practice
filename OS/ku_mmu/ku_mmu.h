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
    char PFN;
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
void initializePage(KU_PTE *);
FreeListElement *getTrailerOfFreeList();
FreeListElement *getFreeListElementByPFN(char);
FreeListElement *addFreeListElement(KU_PTE *, char, FreeListElement *, FreeListElement *);
int setFreeListElement(FreeListElement *, KU_PTE *, char, FreeListElement *, FreeListElement *);
void popFreeListElement();
PCB *addPCBElement(char);
int setPCB(PCB *, PCB *, PCB *, char);
PCB *getPCBByPDBR(KU_PTE *);
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
KU_PTE *getNotUsingSwapInfo(char *);
int swapBeetweenPage(KU_PTE *);
int swapPageOut(char);
void setTableEntryWithIndex(KU_PTE *, char, char);
void setTableEntry(KU_PTE *, char);
KU_PTE *getSwapSpacePageBySwapNum(char);
KU_PTE *getPageOrTableByPFN(char);
char getSwapNumByEntry(char);
char getEntryBySwapNum(char);
char getEntryByPFN(char);
char getPFNByEntry(char);
int mappingProcess(KU_PTE *, char);
int ku_page_fault(char, char);
void *ku_mmu_init(unsigned int, unsigned int);
int ku_run_proc(char, KU_PTE **);

int testflag = 0;
unsigned int pmemSize;
unsigned int swapSize;
void printAllPmemAndSwap()
{
    KU_PTE *tmp = pmem;
    while (tmp - pmem < pmemSize)
    {
        printf("table%d : %d %d %d %d\n", (tmp - pmem) / 4, tmp->entry, (tmp + 1)->entry, (tmp + 2)->entry, (tmp + 3)->entry);
        tmp += PAGE_OFFSET;
    }
    printf("\n");
    tmp = swapSpace;
    while (tmp - swapSpace < swapSize)
    {
        printf("swap%d : %d %d %d %d\n", (tmp - swapSpace) / 4, tmp->entry, (tmp + 1)->entry, (tmp + 2)->entry, (tmp + 3)->entry);
        tmp += PAGE_OFFSET;
    }
    printf("\n");
}
void printAllFreeList()
{
    FreeListElement *tmp = freeListHeader;
    printf("free list : ");
    while (tmp != NULL)
    {
        printf("%d ", tmp->PFN);
        tmp = tmp->next;
    }
    printf("\n");
}
/* handle page fault by 'demand page' or 'swapping(FIFO)'. Page Directory and Page Table not swapped out */
void *ku_mmu_init(unsigned int mem_size, unsigned int swap_size) /* initialize resource. called only once in starting. */
{
    /*1. Allocate memory space for pmem */
    printf("pmem size : %d, swap size : %d\n", mem_size, swap_size);
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
    printf("differ between pmem~swap : %d\n", swapSpace - pmem);
    printAllPmemAndSwap();
    pmemSize = mem_size;
    swapSize = swap_size;
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
    printf("page directory test: %d %d %d %d \n",
           PCBByPid->PDBR->entry,
           ((PCBByPid->PDBR) + 1)->entry,
           ((PCBByPid->PDBR) + 2)->entry,
           ((PCBByPid->PDBR) + 3)->entry);
    if (mappingProcess(PCBByPid->PDBR, va))
        return 0;
    return -1; /* else return -1 */
}
void initializeSwapSpace(unsigned int swap_size)
{
    KU_PTE *tmp = swapSpace;
    swapSpace->entry = -1;
    (swapSpace + 1)->entry = -1;
    (swapSpace + 2)->entry = -1;
    (swapSpace + 3)->entry = -1;
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
    pmem->entry = -1;
    (pmem + 1)->entry = -1;
    (pmem + 2)->entry = -1;
    (pmem + 3)->entry = -1;
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
void initializePage(KU_PTE *pte)
{
    for (int i = 0; i < PAGE_OFFSET; i++)
    {
        pte->entry = -i;
        pte++;
    }
}
int mappingProcess(KU_PTE *pageDirectory, char va) /* map Page Directory ~ Page to pmem */
{
    KU_PTE *PDE, *PMDE, *PTE;
    char *pageIndexes = getPageIndexesByVA(va);
    printf("----Top OF Mapping Process----\n");
    printf("pageIndexes : %d %d %d %d\n", pageIndexes[0], pageIndexes[1], pageIndexes[2], pageIndexes[3]);
    if (pageIndexes == NULL)
        return 0;
    printAllPmemAndSwap();
    printAllFreeList();
    PDE = pageDirectory + pageIndexes[PDE_INDEX]; /* Search Page Directory Entry */
    /* page directory processing : selectedPTE = Page Directory */
    if (getPTEState(PDE) == INVALID) /* if searched PTE is INVALID, */
    {
        mapTable(PDE); /* map PTE referenced by selectedPTE */
        printf("mapping process middle dir test\n");
    }
    /* printf("get Middle Directory addr by PFN : %p\n", getPageOrTableByPFN(getPFNByEntry(PDE->entry))); */
    printAllPmemAndSwap();
    PMDE = getPageOrTableByPFN(getPFNByEntry(PDE->entry)) + /* Search Page Middle Directory entry */
           pageIndexes[PMDE_INDEX];
    /* page middle directory processing : selectedPTE = Page Middle Directory*/
    if (getPTEState(PMDE) == INVALID) /* if searched PTE is INVALID, */
    {
        printf("mapping process table test\n");
        mapTable(PMDE);
    } /* map PTE referenced by selectedPTE */
    /* printf("get Table addr by PFN : %p\n", getPageOrTableByPFN(getPFNByEntry(PMDE->entry))); */
    printAllPmemAndSwap();
    PTE = getPageOrTableByPFN(getPFNByEntry(PMDE->entry)) + /* Search Page Directory entry */
          pageIndexes[PTE_INDEX];
    /* page table processing : selectedPTE = Page Table*/
    if (getPTEState(PTE) == INVALID) /* if searched PTE is INVALID, */
    {
        printf("mapping process page test\n");
        mapPage(PTE);
    } /* map page referenced by selectedPTE */
    else if (getPTEState(PTE) == SWAPPED)
        swapBeetweenPage(PTE);

    /* 해당 경우는 mapping 하려는 page가 swapped 되어 있고, pmem이 꽉차있는 경우여서 page간 swapping 해야함 */
    /* char notUsingPFN;
        int notUsingPFNLocation = getNotUsingPFN(&notUsingPFN); */

    /* printf("get Page addr by PFN : %p\n", getPageOrTableByPFN(getPFNByEntry(PTE->entry))); */
    printAllPmemAndSwap();
    printAllFreeList();
    free(pageIndexes);
    printf("----Bottom OF Mapping Process----\n");
    return 1;
}
KU_PTE *getNotUsingSwapInfo(char *notUsingSwapNum)
{
    char swapNum = 1;
    KU_PTE *tmpSwapSpace = getSwapSpacePageBySwapNum(swapNum);
    while (swapNum < swapSize / 4)
    {
        if (tmpSwapSpace->entry == 3)
        {
            *notUsingSwapNum = swapNum;
            return tmpSwapSpace;
        }
        swapNum++;
        tmpSwapSpace += PAGE_OFFSET;
    }
}
int getNotUsingPFN(char *notUsingPFN) /* Iterating pmem, find default state page. If no page of default, return FreeList's header PFN and move header to next */
{
    char PFN = 1;
    KU_PTE *tmpPmem = getPageOrTableByPFN(PFN);
    while (PFN < pmemSize / 4) /* iterate pmem in first (for default pmem) */
    {
        if (tmpPmem->entry == 3)
        {
            *notUsingPFN = PFN;
            printf("IN_PMEM_LIST test\n");
            return IN_PMEM;
        }
        PFN++;
        tmpPmem += PAGE_OFFSET;
    }
    if (freeListHeader != NULL)
        PFN = freeListHeader->PFN;
    else
    {
        perror("no available mem space");
        exit(0);
    }
    /* freeListHeader = freeListHeader->next;
    free(freeListHeader->prev); */
    *notUsingPFN = PFN;
    printf("IN_FREE_LIST test\n");
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
    /* 2. Allocate Directory to pmem with no-use-PFN (pmem's entry = PDBR)*/
    KU_PTE *notUsingPmem = getPageOrTableByPFN(notUsingPFN);
    char newEntry = getEntryByPFN(notUsingPFN);
    getPCBByPDBR(PDBR)->PFN = notUsingPFN;
    printf("Not Using PFN : %d\n", notUsingPFN);
    printf("newEntry : %d\n", newEntry);
    initializeTable(PDBR);
    if (notUsingPFNLocation == IN_PMEM)
        setDirToPmem(notUsingPmem, PDBR);
    else
    {
        swapPageOut(notUsingPFN);
        setDirToPmem(notUsingPmem, PDBR);
    }
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
    printf("newEntry : %d\n", newEntry);
    /* test */
    /* 3. Allocate that entry to parent's PTE*/
    setTableEntry(parentPTE, newEntry);
    /* if (getPCBByPDBR(parentPTE) == NULL)
        setTableEntry(parentPTE, newEntry);
    else
        setTableEntry(getPageOrTableByPFN(getPCBByPDBR(parentPTE)->PFN), newEntry); */

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
        swapPageOut(notUsingPFN);
        setTableToPmem(notUsingPmem, newTable);
    }
    free(newTable);
    return 1;
}
int mapPage(KU_PTE *parentPTE)
{
    /* 1. Get PFN of being not used space or FIFO-page(getNoUsingPFN function) */
    testflag++;
    char notUsingPFN;
    int notUsingPFNLocation = getNotUsingPFN(&notUsingPFN);
    KU_PTE *notUsingPmem = getPageOrTableByPFN(notUsingPFN);
    /* 2. Get Entry of PFN found in seq 1*/
    char newEntry = getEntryByPFN(notUsingPFN);
    /* test */
    printf("Not Using PFN : %d\n", notUsingPFN);
    printf("newEntry : %d\n", newEntry);
    /* test */
    /* 3. Allocate that entry to parent's PTE*/
    setTableEntry(parentPTE, newEntry);
    /* 4. Make new page : KU_PTE[4]*/
    KU_PTE *newPage = (KU_PTE *)malloc(sizeof(KU_PTE) * PAGE_OFFSET);
    /* 5. Initialize new Table */
    initializePage(newPage);
    /* 6. if 'not using location' is in pmem, allocate new table to pmem and add Free-list element with PFN*/
    if (notUsingPFNLocation == IN_PMEM)
    {
        setPageToPmem(notUsingPmem, newPage);
        addFreeListElement(parentPTE, notUsingPFN, NULL, getTrailerOfFreeList());
    }
    /* 7. if 'not using location' is in free-list, swap it to swapSpace and allocate new table to pmem and Free-list element with PFN */
    else
    {
        swapPageOut(notUsingPFN);
        printf("FREE_LIST else test");
        setPageToPmem(notUsingPmem, newPage);
        addFreeListElement(parentPTE, notUsingPFN, NULL, getTrailerOfFreeList());
    }
    free(newPage);
    printf("testFlag = %d\n", testflag);
    return 1;
}
void setPageToPmem(KU_PTE *notUsingPmem, KU_PTE *page)
{
    for (int i = 0; i < PAGE_OFFSET; i++)
        (notUsingPmem + i)->entry = (page + i)->entry;
}

int swapBeetweenPage(KU_PTE *swapSpaceParentPTE)
{
    /* 1. get swap space by swapSpaceParentPTE*/
    printf("swap beetween page\n");
    KU_PTE *swapSpacePage = getSwapSpacePageBySwapNum(getSwapNumByEntry(swapSpaceParentPTE->entry));
    /* 2. get notUsingPFN in pmem*/
    char notUsingPFN;
    int notUsingPFNLocation = getNotUsingPFN(&notUsingPFN);
    /* 3-1. get and remove notUsingPFN's parentPTE in free-list*/
    FreeListElement *FLEByPFN = getFreeListElementByPFN(notUsingPFN);
    KU_PTE *notUsingParentPTE;
    notUsingParentPTE = FLEByPFN->parentPTE;
    /* if (FLEByPFN != NULL) */
    /* else
        perror("no available mem space_2"); */
    /* 3-2. get notUsingPage in pmem */
    KU_PTE *notUsingPage = getPageOrTableByPFN(notUsingPFN);
    /* 4. swap entry between swapSpaceParentPTE and notUsingParentPTE*/
    char tmpEntry = swapSpaceParentPTE->entry;
    setTableEntry(swapSpaceParentPTE, notUsingParentPTE->entry);
    setTableEntry(notUsingParentPTE, tmpEntry);
    /* 5. swap 2page's entry */
    for (int i = 0; i < PAGE_OFFSET; i++)
    {
        tmpEntry = (swapSpacePage + i)->entry;
        setTableEntry(swapSpacePage + i, (notUsingPage + i)->entry);
        setTableEntry(notUsingPage + i, tmpEntry);
    }
    /* 6. add FreeListElement for swap-in page */
    addFreeListElement(notUsingPage, notUsingPFN, NULL, getTrailerOfFreeList());
    popFreeListElement();
}
int swapPageOut(char notUsingPFN) /* PTE가 아니라, PFN을 넘겨줘서 free-list search 해야함. */
{
    /* 1.getFreeListElementByPFN */
    char notUsingSwapNum;
    FreeListElement *FLEByPFN = getFreeListElementByPFN(notUsingPFN);
    if (FLEByPFN == NULL)
        return 0;
    /* 2.get parentPTE from free-list-element gotten in seq 1 */
    KU_PTE *parentPTE = FLEByPFN->parentPTE;
    /* 3.get Swap Space which is not used(getNotUsingSwapInfo) */
    KU_PTE *notUsingSwapSpace = getNotUsingSwapInfo(&notUsingSwapNum);
    /* 4.set parentPTE with swap offset which the child page allocated in */
    char newEntry = getEntryBySwapNum(notUsingSwapNum);
    setTableEntry(parentPTE, newEntry);
    /* 5. set page to swap space with swap num*/
    getSwapSpacePageBySwapNum(notUsingSwapNum);
    setPageToPmem(notUsingSwapSpace, getPageOrTableByPFN(notUsingPFN));
    popFreeListElement();

    return 1;
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
KU_PTE *getSwapSpacePageBySwapNum(char swapNum)
{
    return swapSpace + swapNum * PAGE_OFFSET;
}
char getEntryBySwapNum(char swapNum)
{
    return swapNum << 1;
}
char getSwapNumByEntry(char entry)
{
    return entry >> 1;
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
void popFreeListElement()
{
    if (freeListHeader->next != NULL)
    {
        freeListHeader = freeListHeader->next;
        free(freeListHeader->prev);
    }
    else
    {
        free(freeListHeader);
        freeListHeader = NULL;
    }
}
FreeListElement *getTrailerOfFreeList()
{
    FreeListElement *tmp = freeListHeader;
    while (tmp != NULL)
    {
        if (tmp->next == NULL)
            return tmp;
        tmp = tmp->next;
    }
    return tmp;
}
FreeListElement *getFreeListElementByPFN(char PFN)
{
    FreeListElement *tmp = freeListHeader;
    while (tmp != NULL)
    {
        if (tmp->PFN == PFN)
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
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
    mapDirectory(newPCB->next->PDBR);
    return newPCB->next;
}
PCB *getPCBByPDBR(KU_PTE *PDBR)
{
    PCB *tmp = PCBHeader;
    while (tmp != NULL)
    {
        if (tmp->PDBR == PDBR)
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
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
    /* else if (PTE->entry & 0x03)
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