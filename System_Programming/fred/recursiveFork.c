#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int recursiveFork(int, int, int, int);
int recursiveWait(int, int);
int main(void)
{
    pid_t rootPid = getpid();
    recursiveFork(0, 5);
    return 0;
}

int recursiveFork(int level, int maxLevel, int processNum, int rootPid)
{
    char *fileName = "dataset";
    if (level == maxLevel)
        return level;
    recursiveFork(level + 1, maxLevel, processNum, rootPid);
    fork(); //2^(maxLevel-level) process   ex)max=5 level=3 -> process 2^(5-3) = 4
    for (i = 0; i < processNum - power(2, maxLevel); i++)
        if (rootPid == getpid())
            fork();
    /* code here(file IO)*/
    recursiveWait(level, processNum); //Does process killed when function finished?
    return level;
}

int recursiveWait(int level, int processNum)
{
}

int power(int x, int y)
{
    int i;
    int temp = x;
    for (i = 0; i < y; i++)
        x *= temp;
    return x;
}