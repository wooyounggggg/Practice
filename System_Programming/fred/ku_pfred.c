#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>

#define MAX_SIZE 9999
#define NAME "/m_queue"

int getFirstLine(int, int *);
int getValueByLineNum(int, int, int);
void recordIntervalArray(int *, int, int, int, const char *);
int decideArrayIndex(int, int);
void printIntervalArray(int *, int, int);
void sendMsg(int *, int, int);
int processSentMsg(int *, int, int);
int main(int argc, char *argv[])
{
    int numOfProcesses;
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
    int sum = 0;

    if (argc <= 1)
    {
        printf("Error : need 4 inputs.\n");
        exit(1);
    }
    numOfProcesses = atoi(argv[1]);
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
    for (processNum = 0; processNum < numOfProcesses; processNum++)
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
        recordIntervalArray(intervalArray, arrayLength, processNum, numOfProcesses, fileName);
    }
    else
    {
        for (i = 0; i < numOfProcesses; i++)
            wait(NULL);
        numOfProcesses = numOfProcesses < maxLineNum ? numOfProcesses : maxLineNum;
        processSentMsg(intervalArray, arrayLength, numOfProcesses);
        for (i = 0; i < arrayLength; i++)
        {
            printf("%d\n", intervalArray[i]);
            sum += intervalArray[i];
        }
    }
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

void recordIntervalArray(int *intervalArray, int arrayLength, int processNum, int numOfProcesses, const char *fileName)
{
    int i;
    int lineNum;
    int offset;
    int processSize;
    int maxLineNum;
    const int interval = intervalArray[0];
    char tmp[20];
    int fd;
    if ((fd = open(fileName, O_RDONLY)) < 0)
    {
        printf("file open failed\n");
        exit(1);
    }
    getFirstLine(fd, &maxLineNum);
    if (!(offset = maxLineNum / numOfProcesses)) //if the number of processes >= the number of value in file, each process' process 1 line with offset 1
    {
        processSize = 1;
        offset = 1;
    }
    else if (processNum == numOfProcesses - 1) //The last process processes rest value of file
        processSize = offset + (maxLineNum % numOfProcesses);
    else //Other processes process file values ​​by the given offset.
        processSize = offset;
    if (processNum < maxLineNum) // if the number of processes is over maxLineNum, 1~maxLineNum processes process file and other processes(maxLineNum+1~) do nothing.
    {
        for (i = 0; i < arrayLength; i++) //initialize intervalArray
            intervalArray[i] = 0;
        for (i = 0; i < processSize; i++)
            intervalArray[decideArrayIndex(getValueByLineNum(fd, offset * processNum + i + 1, maxLineNum), interval)]++;
        sendMsg(intervalArray, arrayLength, numOfProcesses);
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

// void sendMsg(int *intervalArray, int arrayLength, int numOfProcesses)
// {
//     struct mq_attr attr;
//     unsigned int prio = 0;
//     mqd_t mqdes;
//     int i;
//     const int MESSAGE_PER_PROCESS = arrayLength > 2000 ? 5 : 1;
//     int *divideArray[MESSAGE_PER_PROCESS];
//     int dividedLength = arrayLength / MESSAGE_PER_PROCESS;
//     for (i = 0; i < MESSAGE_PER_PROCESS; i++)
//     {
//         if (i == MESSAGE_PER_PROCESS - 1)
//             dividedLength += arrayLength % MESSAGE_PER_PROCESS;
//         divideArray[i] = (int *)malloc(sizeof(int) * dividedLength);
//     }
//     dividedLength -= arrayLength % MESSAGE_PER_PROCESS;
//     attr.mq_maxmsg = numOfProcesses * MESSAGE_PER_PROCESS;
//     attr.mq_msgsize = 8000; /*arrayLength * 4;*/
//     if ((mqdes = mq_open(NAME, O_CREAT | O_WRONLY | O_NONBLOCK, 0666, &attr)) < 0)
//     {
//         perror("mq_open()");
//         exit(0);
//     }
//     for (prio = 0; prio < MESSAGE_PER_PROCESS; prio++)
//     {
//         if (prio == MESSAGE_PER_PROCESS - 1)
//             dividedLength += arrayLength % MESSAGE_PER_PROCESS;
//         if (mq_send(mqdes, (char *)intervalArray, dividedLength, prio) == -1)
//             perror("mq_send()");
//     }

//     mq_close(mqdes);
// }
void sendMsg(int *intervalArray, int arrayLength, int numOfProcesses)
{
    struct mq_attr attr;
    unsigned int prio = 0;
    const int MESSAGE_PER_PROCESS = arrayLength > 2000 ? 5 : 1;
    int dividedLength = (arrayLength / MESSAGE_PER_PROCESS);
    mqd_t mqdes;
    attr.mq_maxmsg = numOfProcesses * MESSAGE_PER_PROCESS;
    attr.mq_msgsize = (dividedLength + arrayLength % MESSAGE_PER_PROCESS) * 4; /*8000*/
    if ((mqdes = mq_open(NAME, O_CREAT | O_WRONLY, 0666, &attr)) < 0)
    {
        perror("mq_open()");
        exit(0);
    }
    for (prio = 0; prio < MESSAGE_PER_PROCESS; prio++)
    {
        if (prio == MESSAGE_PER_PROCESS - 1)
            dividedLength += arrayLength % MESSAGE_PER_PROCESS;
        // printf("prio : %d, dividedLength : %d\n", prio, dividedLength);
        // printf("atrr msgsize : %d\n", attr.mq_msgsize);
        // printf("prio : %d\n", prio);
        // printf("arr len : %d\n", arrayLength);
        if (mq_send(mqdes, (char *)(intervalArray + (arrayLength / MESSAGE_PER_PROCESS) * prio), dividedLength * 4, prio) == -1)
            perror("mq_send()");
    }
    mq_close(mqdes);
}

int processSentMsg(int *intervalArray, int arrayLength, int numOfProcesses)
{
    struct mq_attr attr;
    int *receivedArray;
    unsigned int prio;
    int sum[5] = {0};
    mqd_t mqdes;
    int i;
    const int MESSAGE_PER_PROCESS = arrayLength > 2000 ? 5 : 1;
    int dividedLength = arrayLength / MESSAGE_PER_PROCESS;
    // attr.mq_maxmsg = 1;
    attr.mq_maxmsg = numOfProcesses * MESSAGE_PER_PROCESS;
    attr.mq_msgsize = (dividedLength + arrayLength % MESSAGE_PER_PROCESS) * 4;
    // attr.mq_msgsize = arrayLength * 4;
    // printf("message per process : %d\n", MESSAGE_PER_PROCESS);
    receivedArray = (int *)malloc(sizeof(int) * attr.mq_msgsize);
    if ((mqdes = mq_open(NAME, O_RDWR | O_CREAT | O_NONBLOCK, 0666, &attr)) < 0)
    {
        perror("open()");
        exit(0);
    }
    for (i = 0; i < arrayLength; i++)
        intervalArray[i] = 0;
    // for (i = 0; i < attr.mq_msgsize; i++)
    //     printf("r[%d] : %d\n", i, receivedArray[i]);
    while (mq_receive(mqdes, (char *)receivedArray, (dividedLength + arrayLength % MESSAGE_PER_PROCESS) * 4, &prio) != -1)
    {
        mq_getattr(mqdes, &attr);
        if (MESSAGE_PER_PROCESS == 5)
        {
            // printf("test\n");
            for (i = 0; i < dividedLength + (prio == 4) * (arrayLength % MESSAGE_PER_PROCESS); i++)
            {
                // printf("prio : %d, r[%d] : %d\n", prio, i, receivedArray[i]);
                intervalArray[prio * dividedLength + i] += receivedArray[i];
                sum[prio] += receivedArray[i];
                // if (prio == 4)
                // printf("rec[%d] = %d\n", i, receivedArray[i]);
                // printf("prio : %d, idx : %d\n", prio, prio * dividedLength + i);

                // printf("prio : %d, receivedArray[%d] = %d\n", prio, i, receivedArray[i]);
            }
            // printf("sum[%d]:%d\n", prio, sum[prio]);

            // printf("\n");
        }
        else
            for (i = 0; i < arrayLength; i++)
                intervalArray[i] += receivedArray[i];
    }

    mq_close(mqdes);
    mq_unlink(NAME);
}