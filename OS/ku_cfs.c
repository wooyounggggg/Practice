#include "ku_cfs.h"

ProcessStructure *increasePCB(ProcessStructure *);
int setProcessStructure(ProcessStructure *);
int sortPCB(ProcessStructure *);
double niceValue[NUM_OF_NICE_GROUP] = {0.64, 0.8, 1, 1.25, 1.5625};
/* 도식화 먼저 할 것 */
int main(int argc, char *argv[])
{
    ProcessStructure *PCB;
    int i, j;
    pid_t pid;
    int ts = atoi(argv[6]); /* time slice */
    char charParameter[2] = "A";
    int n[NUM_OF_NICE_GROUP] = {
        atoi(argv[1]),
        atoi(argv[2]),
        atoi(argv[3]),
        atoi(argv[4]),
        atoi(argv[5])}; /* nx */

    /* make child process */
    for (i = 0; i < NUM_OF_NICE_GROUP; i++) /* Iterating for NUM_OF_NICE_GROUP, */
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
                /* 0. STOP child -> in ku_app.c */

                /* 1. increase PCB(make last PCB's next Process) */
                /* 2. insert (pid -> Process's pid / vruntime -> Process's vruntime / niceValue -> Process's niceValue) */
                increasePCB(PCB);
            }
            charParameter[0]++;
        }
        /* 1. sleep(5) */
        sleep(5);
        if (pid == 0)
            break;
    }
    return 0;
}

ProcessStructure *increasePCB(ProcessStructure *PCB)
{
    ProcessStructure *tmp = PCB;
    if (tmp == NULL) /* if PCB has no elements, init PCB */
    {
        if (!(tmp = (ProcessStructure *)malloc(sizeof(ProcessStructure))))
            exit(0);
        setProcessStructure(tmp);
        return tmp;
    }
    while (tmp->next != NULL) /* else increase PCB */
        tmp = tmp->next;
    if (!(tmp->next = (ProcessStructure *)malloc(sizeof(ProcessStructure))))
        exit(0);
    setProcessStructure(tmp->next);
    sortPCB;
    return tmp->next;
}
int setProcessStructure(ProcessStructure *PCB) {}
int sortPCB(ProcessStructure *PCB)
{
    int i;
}