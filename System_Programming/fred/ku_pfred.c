#include <stdio.h>
#include <windows.h>
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
    const char *fileName = argv[3];

    if (argc == 1)
    {
        printf("Error : There's no input!\n");
        exit(1);
    }
    processNum = atoi(argv[1]);
    interval = atoi(argv[2]);
    numOfInterval = MAX_SIZE / interval + 1;
    intervalArray = (int *)malloc(sizeof(int) * numOfInterval);
    if ((fp = fopen(fileName, "r")) == NULL)
    {
        printf("file open failed\n");
        exit(1);
    }
    for (processNumChecker = 0; processNumChecker < processNum; processNumChecker++)
        if ((pid = fork()) == 0)
            break;
        else if (pid < 0)
            perror("fork error\n");

    free(intervalArray);
    fclose(fp);
    if (pid != 0)
        for (i = 0; i < processNum - 1; i++)
            wait(NULL);

    return 0;
}
