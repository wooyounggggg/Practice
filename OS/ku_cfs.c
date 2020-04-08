#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#define NUM_OF_NICE_GROUP 5
typedef struct Process
{
    struct Process *next;
    struct Process *prev;
    double vruntime;
    double niceValue;
    pid_t pid;
} Process;

int addNextProcess(Process *);
int initPCB(Process *);
int sortPCB(Process *);
double niceValue[NUM_OF_NICE_GROUP] = {0.64, 0.8, 1, 1.25, 1.5625};
/* 도식화 먼저 할 것 */
int main(int argc, char *argv[])
{
    Process *PCB;
    int i, j;
    pid_t pid;
    int ts = atoi(argv[6]); /* time slice */
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
                /* excute execl with parameter character */

                break;
            }
            else if (pid < 0) /* fork error */
            {
                perror("fork error\n");
                return 1;
            }
            else /* if ku_cfs scheduler(Parent), increase PCB of 1, save forked child's pid and vruntime to last PCB element*/
            {
                /*
                0. STOP child
                1. sleep(5) 
                2. increase PCB(make last PCB's next Process)
                3. insert (pid -> Process's pid / vruntime -> Process's vruntime / niceValue -> Process's niceValue)
                */
            }
        }
        if (pid == 0)
            break;
    }
    return 0;
}

int addNextProcess(Process *PCB)
{
    Process *tmp = PCB;
    while (tmp->next != NULL)
        tmp = tmp->next;
}

int initPCB(Process *PCB)
{
}
int sortPCB(Process *PCB)
{
    int i;
}