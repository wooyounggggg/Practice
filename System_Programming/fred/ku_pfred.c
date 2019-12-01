#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>

#define MAX_SIZE 9999
#define MSG_SIZE 8
#define DEFAULT_NAME "/m_queue"
// #define MAX_PRIO 32

int getFirstLine(int, int *);
int getValueByLineNum(int, int, int);
void recordIntervalArray(int *, int, int, int);
int decideArrayIndex(int, int);
int combineArray();
void printIntervalArray(int *, int, int);
void sendMsg(int, int *, int, int);
int main(int argc, char *argv[])
{
    int numOfProcess;
    int interval;
    int processNum;
    int *intervalArray;
    int arrayLength;
    int maxLineNum;
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
    arrayLength = MAX_SIZE / interval + 1;
    intervalArray = (int *)malloc(sizeof(int) * arrayLength);
    if ((fd = open(fileName, O_RDONLY)) < 0)
    {
        printf("file open failed\n");
        exit(1);
    }
    getFirstLine(fd, &maxLineNum);
    close(fd);
    // getFirstLine(fd, &maxLineNum);
    // for(i=1;i<=maxLineNum;i++)
    // {
    //     printf("Line %d : %d\n",i,getValueByLineNum(fd,i, numOfLine));
    // }
    for (processNum = 0; processNum < numOfProcess; processNum++)
        if ((pid = fork()) == 0)
            break;
        else if (pid < 0)
        {
            perror("fork error\n");
            return 1;
        }

    // printf("%d\n", fd);
    if (pid == 0)
    {
        intervalArray[0] = interval;
        recordIntervalArray(intervalArray, arrayLength, processNum, numOfProcess);
        // for(i=0;i<arrayLength;i++)
        //     printf("%d\n",intervalArray[i]);
    }
    else
    {
        for (i = 0; i < numOfProcess; i++)
            wait(NULL);
        numOfProcess = numOfProcess < maxLineNum ? numOfProcess : maxLineNum;
        // printf("%d\n", numOfProcess);
        // for (i = 0; i < numOfProcess; i++)
        //     sendMsg(processNum, intervalArray, arrayLength, numOfProcess);
    }
    combineArray();
    free(intervalArray);
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

void recordIntervalArray(int *intervalArray, int arrayLength, int processNum, int numOfProcess)
{
    int i;
    int lineNum;
    int offset;
    int processSize;
    int maxLineNum;
    const int interval = intervalArray[0];
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
    if (processNum < maxLineNum) // if the number of processes is over maxLineNum, 1~maxLineNum processes process file and other processes(maxLineNum+1~) do nothing.
    {
        for (i = 0; i < arrayLength; i++) //initialize intervalArray
            intervalArray[i] = 0;
        // printf("processNum : %d, processSize : %d\n",processNum,processSize);
        for (i = 0; i < processSize; i++)
            intervalArray[decideArrayIndex(getValueByLineNum(fd, offset * processNum + i + 1, maxLineNum), interval)]++;
        sendMsg(processNum, intervalArray, arrayLength, numOfProcess);
        // printIntervalArray(intervalArray, arrayLength, processNum);
        // printf("processNum : %d, line %d : %d(arrayIdx=%d)\n", processNum + 1, offset * processNum + i + 1, getValueByLineNum(fd, offset * processNum + i + 1, maxLineNum), decideArrayIndex(getValueByLineNum(fd, offset * processNum + i + 1, maxLineNum), interval));
    }
}
void printIntervalArray(int *intervalArray, int arrayLength, int processNum)
{
    int i;
    for (i = 0; i < arrayLength; i++)
    {
        printf("process %d(size %d, idx %d) : %d\n", processNum, arrayLength, i, intervalArray[i]);
    }
}
int decideArrayIndex(int num, int interval)
{
    return num / interval;
}

void sendMsg(int processNum, int *intervalArray, int arrayLength, int numOfProcess)
{
    struct mq_attr attr;
    unsigned int prio = 0;
    mqd_t mqdes;
    attr.mq_maxmsg = numOfProcess;
    attr.mq_msgsize = arrayLength * 4;
    if ((mqdes = mq_open(DEFAULT_NAME, O_CREAT | O_WRONLY, 0666, &attr)) < 0)
    {
        perror("mq_open()");
        exit(0);
    }
    // printf("Sending a message %d with priority %d\n", intervalArray, prio);
    if (mq_send(mqdes, (char *)intervalArray, attr.mq_msgsize, prio) == -1)
        perror("mq_send()");

    mq_close(mqdes);
    printf("process %d send finished\n", processNum);
}
int combineArray()
{
    // printf("1\n");
}