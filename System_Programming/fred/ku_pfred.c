#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAX_SIZE 9999

int getFirstLine(int, int *);
int getLineByLineNum(int, int, int);
void recordIntervalArray(int *, int, int, int);
int main(int argc, char *argv[])
{
    int numOfProcess;
    int interval;
    int processNum;
    int *intervalArray;
    int arraySize;
    int numOfLine;
    int i;
    int fd;
    int processSize;
    pid_t pid;
    const char *fileName = argv[3];

    if (argc <= 1)
    {
        printf("Error : need 4 inputs.\n");
        exit(1);
    }
    numOfProcess = atoi(argv[1]);
    interval = atoi(argv[2]);
    arraySize = MAX_SIZE / interval + 1;
    intervalArray = (int *)malloc(sizeof(int) * arraySize);
    if ((fd = open(fileName, O_RDONLY)) < 0)
    {
        printf("file open failed\n");
        exit(1);
    }
    getFirstLine(fd, &numOfLine);
    for (processNum = 0; processNum < numOfProcess; processNum++)
        if ((pid = fork()) == 0)
            break;
        else if (pid < 0)
        {
            perror("fork error\n");
            processNum--;
        }

    intervalArray[0] = fd;
    // printf("%d\n", fd);
    if (pid == 0)
        recordIntervalArray(intervalArray, arraySize, processNum, numOfProcess);
    free(intervalArray);
    // close(fd);
    if (pid != 0)
    {
        for (i = 0; i < numOfProcess; i++)
            wait(NULL);
    }
    return 0;
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

int getLineByLineNum(int fd, int lineNum, int maxNum)
{
    char *tmp;
    int tmpInt;
    int firstLineBytes;
    if (lineNum <= 0 || lineNum > maxNum)
        return -1;
    tmp = (char *)malloc(sizeof(100));
    firstLineBytes = getFirstLine(fd, &tmpInt);
    pread(fd, tmp, 5, firstLineBytes + 5 * (lineNum - 1));
    tmpInt = atoi(tmp);
    free(tmp);
    return tmpInt;
}

void recordIntervalArray(int *intervalArray, int arraySize, int processNum, int numOfProcess)
{
    int i;
    int lineNum;
    int offset;
    int processSize;
    char tmp[20];
    int fd /*= intervalArray[0]*/;
    fd = open("dataset", O_RDONLY);
    printf("%d\n", fd);
    getFirstLine(fd, &lineNum);
    offset = lineNum / numOfProcess;
    if (processNum == numOfProcess - 1)
        processSize = offset + (lineNum % numOfProcess);
    else
        processSize = offset;
    for (i = 0; i < arraySize; i++) //initialize interval array
        intervalArray[i] = 0;
    for (i = 0; i < processSize; i++)
    {
    }
}