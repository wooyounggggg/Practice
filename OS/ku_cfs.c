#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
typedef struct Process
{
    struct Process *next;
    struct Process *prev;
    double vruntime;
    pid_t pid;
} Process;

int addNextProcess(Process *);
int initPCB(Process *);
int sortPCB(Process *);
double nice[5] = {0.64, 0.8, 1, 1.25, 1.5625};
/* 도식화 먼저 할 것 */
int main(int argc, char *argv[])
{
    Process *PCB;
    int n[5] = {atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5])};
    int ts = atoi(argv[6]);
    printf("test %d %d %d %d %d %d\n", n[0], n[1], n[2], n[3], n[4], ts);
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