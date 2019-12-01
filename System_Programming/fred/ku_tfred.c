#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
// #include <sys/wait.h>
// #include <sys/types.h>
// #include <sys/stat.h>

#define MAX_SIZE 9999
#define LOCK 0
#define UNLOCK 1
typedef struct PassingData
{
    char *fileName;
    int *intervalArray;
    int arrayLength;
    int numOfThread;
    int threadNum;
    char threadNumSynchronizer;
} PassingData;
int getFirstLine(int, int *);
int getValueByLineNum(int, int, int);
int decideArrayIndex(int, int);
void *threadFunction(void *);
int main(int argc, char *argv[])
{
    PassingData pass;
    int numOfThread = atoi(argv[1]);
    int interval;
    int threadNum = 0;
    int *intervalArray;
    int arrayLength;
    int maxLineNum;
    int i;
    int fd;
    int processSize;
    pthread_t threadArray[numOfThread];
    const char *fileName = argv[3];

    if (argc <= 1)
    {
        printf("Error : need 4 inputs.\n");
        exit(1);
    }
    // numOfThread = atoi(argv[1]);
    interval = atoi(argv[2]);
    arrayLength = MAX_SIZE / interval + 1;
    intervalArray = (int *)malloc(sizeof(int) * arrayLength);
    if ((fd = open(fileName, O_RDONLY)) < 0)
    {
        printf("file open failed\n");
        exit(1);
    }
    getFirstLine(fd, &maxLineNum);
    close(fd);
    for (i = 0; i < numOfThread; i++)
    {
        if (pass.threadNumSynchronizer != UNLOCK)
        {
            i--;
            continue;
        }
        pthread_create(&threadArray[i], NULL, threadFunction, (void *)&pass);
        pass.threadNumSynchronizer = LOCK;
    }
    /*
    for (processNum = 0; processNum < numOfThread; processNum++)
        if ((pid = fork()) == 0)
            break;
        else if (pid < 0)
        {
            perror("fork error\n");
            return 1;
        }
    if (pid == 0)
    {
        intervalArray[0] = interval;
        recordIntervalArray(intervalArray, arrayLength, processNum, numOfThread);
    }
    else
    {
        for (i = 0; i < numOfThread; i++)
            wait(NULL);
        numOfThread = numOfThread < maxLineNum ? numOfThread : maxLineNum;
        processSentMsg(intervalArray, arrayLength, numOfThread);
        for (i = 0; i < arrayLength; i++)
            printf("%d\n", intervalArray[i]);
    }*/
    free(intervalArray);
    return 0;
}
void *threadFunction(void *_pass)
{
    PassingData *pass = (PassingData *)_pass;
    int threadNum;
    while (1)
    {
        if (pass->threadNumSynchronizer == LOCK)
        {
            threadNum = pass->threadNum;
            pass->threadNumSynchronizer = UNLOCK;
            break;
        }
    }
}
int getFirstLine(int fd, int *numOfLine)
{
    int offsetBytes = 0;
    char buf[100];
    while (1)
    {
        pread(fd, buf + offsetBytes, 1, offsetBytes);
        if (buf[offsetBytes] == '\n')
        {
            buf[offsetBytes] == '\0';
            break;
        }
        offsetBytes++;
    }
    *numOfLine = atoi(buf);
    return offsetBytes;
}

int getValueByLineNum(int fd, int lineNum, int maxLineNum)
{
    char *tmp;
    int tmpInt;
    int firstLineBytes;
    if (lineNum <= 0 || lineNum > maxLineNum)
        return -1;
    tmp = (char *)malloc(sizeof(100));
    firstLineBytes = getFirstLine(fd, &tmpInt);
    if (pread(fd, tmp, 5, firstLineBytes + 5 * (lineNum - 1)) == -1)
        return -1;
    tmpInt = atoi(tmp);
    free(tmp);
    return tmpInt;
}

int decideArrayIndex(int num, int interval)
{
    return num / interval;
}