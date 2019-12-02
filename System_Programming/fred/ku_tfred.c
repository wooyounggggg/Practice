#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
// #include <sys/wait.h>
// #include <sys/types.h>
// #include <sys/stat.h>

#define MAX_SIZE 9999
#define THREAD 0
#define MAIN 1

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
typedef struct PassingData
{
    // char *fileName;
    int *intervalArray;
    int fd;
    int arrayLength;
    int numOfThreads;
    int threadNum;
    int interval;
    char threadNumSynchronizer;
} PassingData;
int getFirstLine(int, int *);
int getValueByLineNum(int, int, int);
int decideArrayIndex(int, int);
void *threadProcedure(void *);
void recordIntervalArray(int *, int, int, int, int, int);
int main(int argc, char *argv[])
{
    PassingData pass;
    // int numOfThreads = atoi(argv[1]);
    // int interval;
    // int threadNum = 0;
    // int *intervalArray;
    // int arrayLength;
    int sum = 0;
    int maxLineNum;
    int i;
    int fd;
    int processSize;
    pthread_t threadArray[atoi(argv[1])];
    // const char *fileName = argv[3];
    if (argc <= 1)
    {
        printf("Error : need 4 inputs.\n");
        exit(1);
    }
    /* passing data initialize */
    pass.interval = atoi(argv[2]);
    pass.arrayLength = MAX_SIZE / pass.interval + 1;
    if ((pass.fd = open(argv[3], O_RDONLY)) < 0)
    {
        printf("file open failed\n");
        exit(1);
    }
    pass.intervalArray = (int *)malloc(sizeof(int) * pass.arrayLength);
    pass.numOfThreads = atoi(argv[1]);
    pass.threadNum = 0;
    pass.threadNumSynchronizer = MAIN;
    for (i = 0; i < pass.arrayLength; i++)
        pass.intervalArray[i] = 0;
    /* passing data initialize */

    // numOfThreads = atoi(argv[1]);
    // arrayLength = MAX_SIZE / interval + 1;
    // intervalArray = (int *)malloc(sizeof(int) * arrayLength);
    if ((fd = open(argv[3], O_RDONLY)) < 0)
    {
        printf("file open failed\n");
        exit(1);
    }
    getFirstLine(fd, &maxLineNum);
    close(fd);
    for (i = 0; i < pass.numOfThreads; i++)
    {
        if (pass.threadNumSynchronizer == THREAD) //if the synchronizer variable is THREAD, main thread does nothing and waits for the synchronizer to become MAIN by the thread created just before.
        {
            i--;
            continue;
        }
        pthread_create(&threadArray[i], NULL, threadProcedure, (void *)&pass);
        pass.threadNumSynchronizer = THREAD;
    }
    for (i = 0; i < pass.numOfThreads; i++)
        pthread_join(threadArray[i], NULL);
    for (i = 0; i < pass.arrayLength; i++)
    {
        printf("%d\n", pass.intervalArray[i]);
        sum += pass.intervalArray[i];
    }
    free(pass.intervalArray);
    // printf("sum:%d\n", sum);
    return 0;
}
void *threadProcedure(void *_pass)
{
    PassingData *pass = (PassingData *)_pass;
    int threadNum;
    int fd = pass->fd;
    int maxLineNum;
    // int taskDone = 0;
    getFirstLine(fd, &maxLineNum);
    while (1)
    {
        if (pass->threadNumSynchronizer == MAIN) //if synchronizer variable is MAIN, this thread does nothing and waits for the synchronizer to become THREAD by main thread.
            continue;
        threadNum = pass->threadNum;
        pass->threadNum++;
        pass->threadNumSynchronizer = MAIN;
        break;
    }
    while (1)
    {
        if (pthread_mutex_trylock(&mutex) == 0)
        {
            recordIntervalArray(pass->intervalArray, pass->arrayLength, threadNum, pass->numOfThreads, fd, pass->interval);
            // printf("test\n");
            pthread_mutex_unlock(&mutex);
            // taskDone = 1;
            return 0;
        }
    }
}

void recordIntervalArray(int *intervalArray, int arrayLength, int threadNum, int numOfThreads, int fd, int interval)
{
    int i;
    int lineNum;
    int offset;
    int processSize;
    int maxLineNum;
    char tmp[20];
    getFirstLine(fd, &maxLineNum);
    if (!(offset = maxLineNum / numOfThreads)) //if the number of processes >= the number of value in file, each thread process 1 line with offset 1
    {
        processSize = 1;
        offset = 1;
    }
    else if (threadNum == numOfThreads - 1) //The last process processes rest value of file
        processSize = offset + (maxLineNum % numOfThreads);
    else //Other processes process file values ​​by the given offset.
        processSize = offset;
    if (threadNum < maxLineNum) // if the number of processes is over maxLineNum, 1~maxLineNum threads process file and other processes(maxLineNum+1~) do nothing.
    {
        // printf("%d\n", getValueByLineNum(fd, offset * threadNum + i + 1, maxLineNum), interval);
        for (i = 0; i < processSize; i++)
            intervalArray[decideArrayIndex(getValueByLineNum(fd, offset * threadNum + i + 1, maxLineNum), interval)]++;
        // printf("%d\n", decideArrayIndex(getValueByLineNum(fd, offset * threadNum + i + 1, maxLineNum), interval));
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