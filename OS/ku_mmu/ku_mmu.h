#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* typedef struct ku_pte
{
} KU_PTE; */
typedef struct ku_mmu_PCB
{
    char pid;
    char va;
} ku_mmu_PCB;
/* 0xff */
int ku_page_fault(char pid, char va) /* handle page fault by 'demanding page' or 'swapping' -> FIFO. Page Directory and Page Table not swapped out */
{
    if (1 == 1) /* if success, return 0 */
        return 0;
    return -1; /* else return -1 */
}
void *ku_mmu_init(unsigned int mem_size, unsigned int swap_size) /* initialize resource. called only once in starting. */
{
    if (1 == 1)
        return; /* if success, return the pointer of allocated area. */
    return 0;   /* if fail, return 0 */
}
int ku_run_proc(char pid, struct ku_pte **ku_cr3) /* Performs Context Switch. If pid is new, function creates a process in virtual and its page directory */
{
    /* ku_cr3 -> point to PDE base address */
    /* 
    1. get PCB(global) which has parameter local variable pid(functionally) : getPCBByPid(char pid)
    2. get va of PCB gotten in seq 1
    3. map ku_cr3 to page-dir by Arithmetic Operation with VA gotten in seq 2(functionally) : mapKuCR3(char va)
    */
    if (1 == 1)
        return 0; /* if success, return 0 */
    return -1;    /* if fail, return -1 */
}
int isPresent(char va) /* check if va's LSB of process is 1 and others are not all 0 */
{
    return (va & 1) && (va != 1);
}