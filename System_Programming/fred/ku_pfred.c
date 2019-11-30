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
int decideInterval(int, int);
int combineArray();
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
    // if ((fd = open(fileName, O_RDONLY)) < 0)
    // {
    //     printf("file open failed\n");
    //     exit(1);
    // }
    // getFirstLine(fd, &numOfLine);
    // for(i=1;i<=numOfLine;i++)
    // {
    //     printf("Line %d : %d\n",i,getLineByLineNum(fd,i, numOfLine));
    // }
    for (processNum = 0; processNum < numOfProcess; processNum++)
        if ((pid = fork()) == 0)
            break;
        else if (pid < 0)
        {
            perror("fork error\n");
            processNum--;
        }

    // printf("%d\n", fd);
    if (pid == 0)
    {
        intervalArray[0] = interval;
        recordIntervalArray(intervalArray, arraySize, processNum, numOfProcess);
        // for(i=0;i<arraySize;i++)
        //     printf("%d\n",intervalArray[i]);
    }
    free(intervalArray);
    if (pid != 0)
        for (i = 0; i < numOfProcess; i++)
        {
            // if (combineArray() == -1)
            //     printf("message queue error\n");
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

int getLineByLineNum(int fd, int lineNum, int maxLineNum)
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

void recordIntervalArray(int *intervalArray, int arraySize, int processNum, int numOfProcess)
{
    int i;
    int lineNum;
    int offset;
    int processSize;
    int maxLineNum;
    int interval = intervalArray[0];
    char tmp[20];
    int fd;
    fd = open("dataset", O_RDONLY);
    getFirstLine(fd, &maxLineNum);
    if (!(offset = maxLineNum / numOfProcess)) //if the number of processes >= the number of value in file, each process' process 1 line with offset 1
    {
        processSize = 1;
        offset = 1;
    }
    else if (processNum == numOfProcess - 1) //The last process processes rest value of file
        processSize = offset + (maxLineNum % numOfProcess);
    else //Other processes process file values ​​by the given offset.
        processSize = offset;
    if (processNum + 1 <= maxLineNum) // if the number of processes is over maxLineNum, 1~maxLineNum processes process file and other processes(maxLineNum+1~) do nothing.
    {
        for (i = 0; i < arraySize; i++) //initialize intervalArray
            intervalArray[i] = 0;
        // printf("processNum : %d, processSize : %d\n",processNum,processSize);
        for (i = 0; i < processSize; i++)
        {
            printf("processNum : %d, line %d : %d(arrayIdx=%d)\n", processNum + 1, offset * processNum + i + 1, getLineByLineNum(fd, offset * processNum + i + 1, maxLineNum), decideInterval(getLineByLineNum(fd, offset * processNum + i + 1, maxLineNum), interval));
            // intervalArray[decideInterval(getLineByLineNum(fd, offset * processNum + i + 1, maxLineNum), interval)]++;
        }
    }
}
int decideInterval(int num, int interval)
{
    return num / interval;
}

int combineArray()
{
    return -1;
}