#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#define MAX_SIZE 9999
int main(int argc, char *argv[])
{

    int processNum;
    int processNumChecker;
    int interval;
    int *intervalArray;
    int numOfInterval;
    int i;
    pid_t pid;
    pid_t rootPid = getpid();
    FILE *fp;

    if (argc == 1)
    {
        printf("Error : There's no input!\n");
        exit(1);
    }
    processNum = atoi(argv[1]);
    interval = atoi(argv[2]);
    numOfInterval = MAX_SIZE / interval + 1;
    intervalArray = (int *)malloc(sizeof(int) * numOfInterval);
    const char *fileName = argv[3];
    if ((fp = fopen(fileName, "r")) == NULL)
    {
        printf("file open failed\n");
        exit(1);
    }
    printf("pNum : %d checker : %d\n", processNum, processNumChecker);
    for (processNumChecker = 1; processNumChecker * 2 <= processNum; processNumChecker *= 2)
    {
        pid = fork();
        if (getpid() == rootPid)
            printf("%d\n", processNumChecker);
    }
    if (getpid() == rootPid)
        for (i = 0; i < processNum - processNumChecker; i++)
            fork();
    printf("%d\n", processNumChecker);
    free(intervalArray);
    fclose(fp);
    pid = getpid();
    if (pid == rootPid)
    {
        // processNumChecker = wait(NULL);
        printf("%d process end\n", processNumChecker);
        while (1)
        {
        }
    }
    return 0;
}
