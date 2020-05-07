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
PCB *PCBList = NULL;
struct ku_pte *pmem = NULL;
struct ku_pte *swapSpace = NULL;

PCB *addPCBElement(char pid) /* add Process' PCB element and map PDBR to pmem */
{
    PCB *newPCB = PCBList;
    while (newPCB != NULL)
        newPCB = newPCB->next;
    newPCB = (PCB *)malloc(sizeof(PCB));
    if (newPCB == NULL)
        return NULL;
    newPCB->PDBR = (struct ku_pte *)malloc(sizeof(struct ku_pte) * 4);
    if (newPCB->PDBR == NULL)
    {
        free(newPCB);
        return NULL;
    }
    mapPageOrTable(newPCB->PDBR, TABLE);
    newPCB->prev = NULL;
    newPCB->next = NULL;
    newPCB->pid = pid;
    return newPCB;
}
PCB *getPCBByPid(char pid)
{
    PCB *tmp = PCBList;
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

/* Page Directory와 Page Middle Dir, Page Table 및 Page를 pmem에 적재해야 하는데, Page Directory는 PCB->PDBR로 적재를 하지 va를 통한 indexing으로 적재하지 않기 때문에,
이를 고려하여서 분기로 적재해주거나 따로 함수를 하나 만들어야 함 */
int mapPageOrTable(struct ku_pte *pte, int pageOrTable) /* map page table to pmem */
{
    struct ku_pte *newPageOrTable;
    char PFN = getPFNByEntry(pte->entry);
    if (pageOrTable == PAGE)
        printf("page\n");
    else if (pageOrTable == TABLE)
    {
        printf("table");
    }
}
int swapPageOrTable(struct ku_pte *pte)
{
}
struct ku_pte *getPageTableByPFN(char PFN)
{
    return pmem + PFN * 4;
}
char getPFNByEntry(char entry)
{
    return entry >> 2;
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
        mapPageOrTable(selectedPTE, TABLE);  /* map PTE referenced by selectedPTE */
    else if (getPTEState(selectedPTE) == SWAPPED)
        swapPageOrTable(selectedPTE);
    selectedPTE = getPageTableByPFN(getPFNByEntry(selectedPTE->entry)) + /* Search Page Middle Directory entry */
                  pageIndexes[PMDE_INDEX];
    /* page middle directory processing : selectedPTE = Page Middle Directory*/
    if (getPTEState(selectedPTE) == INVALID) /* if searched PTE is INVALID, */
        mapPageOrTable(selectedPTE, TABLE);  /* map PTE referenced by selectedPTE */
    else if (getPTEState(selectedPTE) == SWAPPED)
        swapPageOrTable(selectedPTE);
    selectedPTE = getPageTableByPFN(getPFNByEntry(selectedPTE->entry)) + /* Search Page Directory entry */
                  pageIndexes[PTE_INDEX];
    /* page table processing : selectedPTE = Page Table*/
    if (getPTEState(selectedPTE) == INVALID) /* if searched PTE is INVALID, */
        mapPageOrTable(selectedPTE, PAGE);   /* map page referenced by selectedPTE */
    else if (getPTEState(selectedPTE) == SWAPPED)
        swapPageOrTable(selectedPTE);
    free(pageIndexes);
}

/* handle page fault by 'demand page' or 'swapping(FIFO)'. Page Directory and Page Table not swapped out */
int ku_page_fault(char pid, char va)
{
    /* 1. get PCB by PID */
    PCB *PCBByPid = getPCBByPid(pid);
    /* 2. Page Directory ~ Page Mapping*/
    mappingProcess(PCBByPid->PDBR, va);
    /* 3. */
    if (1 == 1) /* if success, return 0 */
        return 0;
    return -1; /* else return -1 */
}
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