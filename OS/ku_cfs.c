#include "ku_cfs.h"

PS *increasePCB(const PS *, int, pid_t);
int swapPS(PS *, PS *);
PS *getMinPCB(const PS *);
pid_t schedulePCB(PS *);
int initPS(PS *, int, pid_t);
int renewPS(PS *);
const double niceValue[MAX_NICE_INDEX] = {0.64, 0.8, 1, 1.25, 1.5625};
/* 도식화 먼저 할 것 */
int main(int argc, char *argv[])
{
    PS *PCB;
    int i, j;
    pid_t pid;
    int ts = atoi(argv[6]); /* time slice */
    char charParameter[2] = "A";
    int n[MAX_NICE_INDEX] = {
        atoi(argv[1]),
        atoi(argv[2]),
        atoi(argv[3]),
        atoi(argv[4]),
        atoi(argv[5])}; /* nx */

    /* make child process */
    for (i = 0; i < MAX_NICE_INDEX; i++) /* Iterating for MAX_NICE_INDEX, */
    {
        for (j = 0; j < n[i]; j++) /* make n[j] of processes */
        {
            if ((pid = fork()) == 0) /* if child */
            {
                /* excute execl with parameter character A ~ Z */
                execl("./ku_app", charParameter, charParameter, NULL);
                break;
            }
            else if (pid < 0) /* fork error */
            {
                perror("fork error\n");
                return 1;
            }
            else /* if ku_cfs scheduler(Parent), increase PCB of 1, save forked child's pid, vruntime and niceValue to last PCB element*/
            {
                /* 1. increase PCB(make last PCB's next Process) */
                /* 2. insert (pid -> Process's pid / vruntime -> Process's vruntime / niceValue -> Process's niceValue) */
                increasePCB(PCB, i, pid);
            }
            charParameter[0]++;
        }
    }
    return 0;
}
PS *increasePCB(const PS *PCB, int niceIndex, pid_t pid)
{
    PS *newPS = PCB;
    while (newPS != NULL)
        newPS = newPS->next;
    if (!(newPS = (PS *)malloc(sizeof(PS))))
        exit(0);
    initPS(newPS, niceIndex, pid);
    schedulePCB(PCB);
    return newPS;
}
pid_t schedulePCB(PS *PCB) /* sort PCB */
{
    PS *tmp = PCB;
    PS *min = PCB;
    PS *header = PCB;
    while (tmp != NULL)
    {
    }
}

int swapPS(PS *PS1, PS *PS2)
{
    PS *tmp;
    tmp->prev = PS1->prev;
    tmp->next = PS1->next;
    PS1->prev = PS2->prev;
    PS1->prev =
}
PS *getMinPCB(PS *PCB)
{
    PS *tmp = PCB;
    PS *minPCB = PCB;
    while ((tmp->next) != NULL)
    {
        tmp = tmp->next;
        minPCB = minPCB->vruntime > tmp->vruntime ? tmp : minPCB;
    }
    return minPCB;
}
int initPS(PS *PS, int niceIndex, pid_t pid) /* initialize PS */
{
    PS->vruntime = niceValue[niceIndex]; /* vruntime = vruntime(0) + 1.25^nice */
    PS->niceIndex = niceIndex;
    PS->pid = pid;
}
int renewPS(PS *PS) /* Renew PS's vruntime */
{
    PS->vruntime += niceValue[PS->niceIndex]; /* vruntime = vruntime + 1.25^nice */
}