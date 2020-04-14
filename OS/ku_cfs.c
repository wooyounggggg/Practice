#include "ku_cfs.h"

void printPCB(PS *);
PS *increasePCB(PS *, int, pid_t);
int swapPS(PS *, PS *);
PS *getMinPS(PS *);
pid_t schedulePCB(PS *);
int setPS(PS *, PS *, PS *, int, pid_t);
int renewPS(PS *);
const double niceValue[MAX_NICE_INDEX] = {0.64, 0.8, 1, 1.25, 1.5625};

/* 도식화 먼저 할 것 */
int main(int argc, char *argv[])
{
    PS *PCB = (PS *)malloc(sizeof(PS));
    PCB->next = NULL;
    PCB->prev = NULL;
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
            if ((pid = fork()) == 0) /* if child, */
            {
                execl("./ku_app", charParameter, charParameter, NULL); /* excute execl with parameter character A ~ Z */
                break;
            }
            else if (pid < 0) /* fork error */
            {
                perror("fork error\n");
                return 1;
            }
            else if (i == 0 && j == 0)          /* if Parent and First PCB elements, */
                setPS(PCB, NULL, NULL, i, pid); /* just set PS */
            else                                /* else increase PCB(make last PCB's next Process) and insert PS's structure elements */
                increasePCB(PCB, i, pid);

            charParameter[0]++;
        }
    }
    /* timer와의 연동 -> ts동안 schedule 작업(while) */
    while (1)
    {
    }
    for (i = 0; i < n[0] + n[1] + n[2] + n[3] + n[4]; i++)
        wait(NULL);
    return 0;
}
void printPCB(PS *PCB)
{
    PS *tmp = PCB;
    while (tmp != NULL)
    {
        printf("pid : %d, vruntime : %f, niceIndex : %d\n", tmp->pid, tmp->vruntime, tmp->niceIndex);
        tmp = tmp->next;
    }
    printf("\n\n");
}
PS *increasePCB(PS *PCB, int niceIndex, pid_t pid)
{
    PS *newPS = PCB;
    /* PCB 첫 값을 초기화해야함 */
    while (newPS->next != NULL)
        newPS = newPS->next;
    if (!(newPS->next = (PS *)malloc(sizeof(PS))))
        exit(0);
    setPS(newPS->next, newPS, NULL, niceIndex, pid);
    return newPS;
}
pid_t schedulePCB(PS *PCB) /* PCB selection sort */
{
    const PS *header = PCB;
    PS *point = PCB;
    PS *finder;
    while (point != NULL)
    {
        swapPS(point, getMinPS(point));
        point = point->next;
    }
}

int swapPS(PS *PS1, PS *PS2) /* swap PS */
{
    double vruntimeTmp = PS1->vruntime;
    int niceIndexTmp = PS1->niceIndex;
    pid_t pidTmp = PS1->pid;
    PS1->vruntime = PS2->vruntime;
    PS1->niceIndex = PS2->niceIndex;
    PS1->pid = PS2->pid;
    PS2->vruntime = vruntimeTmp;
    PS2->niceIndex = niceIndexTmp;
    PS2->pid = pidTmp;
}
PS *getMinPS(PS *PCB)
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
int setPS(PS *newPS, PS *prev, PS *next, int niceIndex, pid_t pid) /* initialize PS */
{
    newPS->vruntime = niceValue[niceIndex]; /* vruntime = vruntime(0) + 1.25^nice */
    newPS->niceIndex = niceIndex;
    newPS->pid = pid;
    newPS->next = next;
    newPS->prev = prev;
    return SUCCESS;
}
int renewPS(PS *PS) /* Renew PS's vruntime */
{
    PS->vruntime += niceValue[PS->niceIndex]; /* vruntime = vruntime + 1.25^nice */
    return SUCCESS;
}