#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 0xff */
int ku_page_fault(char pid, char va) /* handle page fault by 'demanding page' or 'swapping' -> FIFO. Page Directory and Page Table not swapped out */
{
    if (1 == 1) /* if success, return 0 */
        return 0;
    return -1; /* else return 1 */
}
void *ku_mmu_init(unsigned int mem_size, unsigned int swap_size) /* initialize resource. called only once. */
{
    if (1 == 1)
        return; /* if success, return the pointer of allocated area. */
    return 0;   /* if fail, return 0 */
}
int ku_run_proc(char pid, struct ku_pte **ku_cr3) /* Performs Context Switch. If pid is new, function creates a process in virtual and its page directory */
{
    /* ku_cr3 -> point to PDE base address */
    if (1 == 1)
        return 0; /* if success, return 0 */
    return -1;    /* if fail, return -1 */
}
int isPresent(char va)
{
    return va & 1;
}