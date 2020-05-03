#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PRESENT 1
#define SWAPPED 0
#define UNMAPPED -1
#define PDE_INDEX 0
#define PMDE_INDEX 1
#define PTE_INDEX 2
#define PAGE_INDEX 3
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

int ku_page_fault(char pid, char va) /* handle page fault by 'demanding page' or 'swapping' -> FIFO. Page Directory and Page Table not swapped out */
{
    if (1 == 1) /* if success, return 0 */
        return 0;
    return -1; /* else return -1 */
}
void *ku_mmu_init(unsigned int mem_size, unsigned int swap_size) /* initialize resource. called only once in starting. */
{
    /* 
    1.init PCBList

    2.
    */
    if (1 == 1)
        return; /* if success, return the pointer of allocated area. */
    return 0;   /* if fail, return 0 */
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
    if (*ku_cr3 == NULL)
        return -1; /* if fail, return -1 */
    return 0;
}

PCB *addPCBElement(char pid)
{
    PCB *newPCB = PCBList;
    while (newPCB != NULL)
        newPCB = newPCB->next;
    newPCB = (PCB *)malloc(sizeof(PCB));
    if (newPCB == NULL)
        return NULL;
    newPCB->PDBR = (struct ku_pte *)malloc(sizeof(struct ku_pte));
    if (newPCB->PDBR == NULL)
    {
        free(newPCB);
        return NULL;
    }
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
int getPTEState(char PTE)
{
    if (PTE == 0)
        return UNMAPPED;
    else if ((PTE & 0x01) && (PTE >> 2)) /* if Present bit is 1 and PFN > 0, return PRESENT */
        return PRESENT;
    else if (!(PTE & 0x01) && (PTE >> 1)) /* if Present bit is 0 and Swap Offset > 0, return SWAPPED */
        return SWAPPED;
}
int getPageIndexByVA(char *pageIndexes, char va)
{
    if (pageIndexes == NULL)
        pageIndexes == (char *)malloc(sizeof(char) * 4);
    if (pageIndexes == NULL)
        return 0;
    pageIndexes[PDE_INDEX] = va >> 6;
    pageIndexes[PMDE_INDEX] = va >> 4 & 0x03;
    pageIndexes[PTE_INDEX] = va >> 2 & 0x03;
    pageIndexes[PAGE_INDEX] = va & 0x03;
    return 1;
}