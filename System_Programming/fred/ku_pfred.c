#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>
#include <string.h>
#include <errno.h>

#define MAX_SIZE 9999
#define DEFAULT_NAME "/m_queue"
#define CHECK 1
#define UNCHECK 0

int getFirstLine(int, int *);
int getValueByLineNum(int, int, int);
void recordIntervalArray(int *, int, int, int, const char *);
int decideArrayIndex(int, int);
void sendMsg(int *, int, int, int);
int processSentMsg(int *, int, int);
void makeFileName(char *, int);
int getLengthSize(int);
int allMessageQueueChecked(int *, int);
// void mergeArray(int *, int *, int, int);
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
        numOfProcesses = numOfProcesses < maxLineNum ? numOfProcesses : maxLineNum;
        for (i = 0; i < arrayLength; i++)
            intervalArray[i] = 0;
        processSentMsg(intervalArray, arrayLength, numOfProcesses);
        for (i = 0; i < numOfProcesses; i++)
            wait(NULL);
        for (i = 0; i < arrayLength; i++)
        {
            // printf("%d\n", intervalArray[i]);
            sum += intervalArray[i];
        }
        printf("sum : %d\n", sum);
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
        sendMsg(intervalArray, arrayLength, numOfProcesses, processNum);
    }
}
int decideArrayIndex(int num, int interval)
{
    return num / interval;
}
void makeFileName(char *fileName, int processNum)
{
    strcpy(fileName, DEFAULT_NAME);
    sprintf(fileName + 8, "%d", processNum);
}
int getLengthSize(int length)
{
    return length * 4;
}
void sendMsg(int *intervalArray, int arrayLength, int numOfProcesses, int processNum)
{
    struct mq_attr attr;
    int value = 0;
    unsigned int prio;
    char msgQueueFileName[13];
    const int MESSAGE_COUNT_PER_PROCESS = arrayLength > 2000 ? 5 : 1;
    int dividedLength = (arrayLength / MESSAGE_COUNT_PER_PROCESS);
    int restLength = arrayLength % MESSAGE_COUNT_PER_PROCESS;
    mqd_t mqdes;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = getLengthSize(dividedLength + restLength);
    makeFileName(msgQueueFileName, processNum);
    if ((mqdes = mq_open(msgQueueFileName, O_CREAT | O_RDWR, 0666, &attr)) < 0)
    {
        perror("mq_open()");
        exit(0);
    }
    for (prio = 0; prio < MESSAGE_COUNT_PER_PROCESS; prio++)
    {
        if (prio == MESSAGE_COUNT_PER_PROCESS - 1)
            dividedLength += restLength;
        if (mq_send(mqdes, (char *)(intervalArray + prio * (arrayLength / MESSAGE_COUNT_PER_PROCESS)), getLengthSize(dividedLength), prio) == -1)
            perror("mq_send()");
    }
    mq_close(mqdes);
}
int processSentMsg(int *intervalArray, int arrayLength, int numOfProcesses)
{
    struct mq_attr attr;
    int *receivedArray;
    unsigned int prio;
    int *messageCheck;
    int sum = 0;
    mqd_t mqdes;
    char msgQueueFileName[13];
    const int MESSAGE_COUNT_PER_PROCESS = arrayLength > 2000 ? 5 : 1;
    int dividedLength = arrayLength / MESSAGE_COUNT_PER_PROCESS;
    int restLength = arrayLength % MESSAGE_COUNT_PER_PROCESS;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = getLengthSize(dividedLength + restLength) + 1;
    int i = 0;
    int j, k;
    receivedArray = (int *)malloc(sizeof(int) * (attr.mq_msgsize));
    messageCheck = (int *)malloc(sizeof(int) * numOfProcesses);
    for (j = 0; j < MESSAGE_COUNT_PER_PROCESS; j++)
        messageCheck[j] = 0;
    //메시지 큐 차례로 열기
    while (!allMessageQueueChecked(messageCheck, numOfProcesses))
    {
        //queue open
        // printf("before allMessageQueueChecked : %d\n", allMessageQueueChecked(messageCheck, numOfProcesses));
        makeFileName(msgQueueFileName, i);
        if (messageCheck[i] == CHECK || (mqdes = mq_open(msgQueueFileName, O_RDWR | O_NONBLOCK, 0666, &attr)) < 0)
        {
            if (messageCheck[i] == UNCHECK)
                mq_close(mqdes);
            i = (i + 1) % numOfProcesses;
            continue;
        }
        // printf("before messageCheck[%d] : %d\n", i, messageCheck[i]);
        // printf("mqueue %d\n", i);
        //메시지 큐 하나에서 메시지 받기
        for (j = 0; j < MESSAGE_COUNT_PER_PROCESS; j++)
        {
            // if (j == MESSAGE_COUNT_PER_PROCESS - 1)
            //     dividedLength += restLength;

            if (mq_receive(mqdes, (char *)receivedArray, getLengthSize(attr.mq_msgsize), &prio) == -1)
            {
                // if (j == MESSAGE_COUNT_PER_PROCESS - 1)
                //     dividedLength -= restLength;
                j--;
                continue;
            }
            if (prio == MESSAGE_COUNT_PER_PROCESS - 1)
                dividedLength += restLength;
            /* received Array를 interval array로 옮겨 담기 */
            if (MESSAGE_COUNT_PER_PROCESS == 5)
                for (k = 0; k < dividedLength; k++)
                {
                    // printf("processNum : %d - %d\n", i, k);

                    // printf("processNum : %d prio : %d, receivedArray[%d] : %d\n", i, prio, k, receivedArray[k]);
                    // sum += receivedArray[k];
                    intervalArray[prio * (arrayLength / MESSAGE_COUNT_PER_PROCESS) + k] += receivedArray[k];
                    // sum[prio] += receivedArray[i];
                    // printf("sum : %d\n", sum);
                }
            else
                for (k = 0; k < arrayLength; k++)
                {
                    intervalArray[k] += receivedArray[k];
                    // sum[prio] += receivedArray[i];
                }
            /* received Array를 interval array로 옮겨 담기 */
            if (prio == MESSAGE_COUNT_PER_PROCESS - 1)
            {
                dividedLength -= restLength;
                messageCheck[i] = CHECK;
            }
        }
        //
        mq_close(mqdes);
        if (messageCheck[i] == CHECK)
            mq_unlink(msgQueueFileName);
        /**/
        // printf("after messageCheck[%d] : %d\n\n", i, messageCheck[i]);
        i = (i + 1) % numOfProcesses;
        // printf("after allMessageQueueChecked : %d\n\n", allMessageQueueChecked(messageCheck, numOfProcesses));
    }
    // printf("test\n");
}
// void mergeArray(int *intervalArray, int *receivedArray, int MESSAGE_COUNT_PER_PROCESS, int prio, int dividedLength)
// {
//     if (MESSAGE_COUNT_PER_PROCESS == 5)
//         for (i = 0; i < dividedLength + (prio == 4) * (arrayLength % MESSAGE_COUNT_PER_PROCESS); i++)
//         {
//             intervalArray[prio * dividedLength + i] += receivedArray[i];
//             sum[prio] += receivedArray[i];
//             printf("prio : %d : %d\n", prio, receivedArray[i]);
//         }
//     else
//         for (i = 0; i < arrayLength; i++)
//         {
//             intervalArray[i] += receivedArray[i];
//             sum[prio] += receivedArray[i];
//             printf("prio : %d : %d\n", prio, receivedArray[i]);
//         }
// }

int allMessageQueueChecked(int *messageCheck, int size)
{
    int i;
    for (i = 0; i < size; i++)
        if (messageCheck[i] == UNCHECK)
            return UNCHECK;
    return CHECK;
}
// void sendMsg(int *intervalArray, int arrayLength, int numOfProcesses)
// {
//     struct mq_attr attr;
//     unsigned int prio = 0;
//     const int MESSAGE_COUNT_PER_PROCESS = arrayLength > 2000 ? 5 : 1;
//     int dividedLength = (arrayLength / MESSAGE_COUNT_PER_PROCESS);
//     mqd_t mqdes;
//     attr.mq_maxmsg = 10;
//     attr.mq_msgsize = (dividedLength + arrayLength % MESSAGE_COUNT_PER_PROCESS) * 4;
//     for (prio = 0; prio < MESSAGE_COUNT_PER_PROCESS; prio++)
//     {
//         if ((mqdes = mq_open(DEFAULT_NAME, O_CREAT | O_WRONLY, 0666, &attr)) < 0)
//         {
//             perror("mq_open()");
//             exit(0);
//         }
//         if (prio == MESSAGE_COUNT_PER_PROCESS - 1)
//             dividedLength += arrayLength % MESSAGE_COUNT_PER_PROCESS;
//         if (mq_send(mqdes, (char *)(intervalArray + (arrayLength / MESSAGE_COUNT_PER_PROCESS) * prio), dividedLength * 4, prio) == -1)
//         {
//             if(errno==EAGAIN)
//             {
//                 if (prio == MESSAGE_COUNT_PER_PROCESS - 1)
//                     dividedLength -= arrayLength % MESSAGE_COUNT_PER_PROCESS;
//                 prio--;
//                 mq_close(mqdes);
//                 continue;
//             } // perror("mq_send()");
//         }
//         mq_close(mqdes);
//         // printf("send : %d\n", prio);
//     }
// }
// }
// printf("send success\n");

/*
int processSentMsg(int *intervalArray, int arrayLength, int numOfProcesses)
{
    struct mq_attr attr;
    int *receivedArray;
    unsigned int prio;
    mqd_t mqdes;
    int i, j;
    const int MESSAGE_COUNT_PER_PROCESS = arrayLength > 2000 ? 5 : 1;
    int dividedLength = arrayLength / MESSAGE_COUNT_PER_PROCESS;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = (dividedLength + arrayLength % MESSAGE_COUNT_PER_PROCESS) * 4;
    receivedArray = (int *)malloc(sizeof(int) * attr.mq_msgsize);
    for (i = 0; i < arrayLength; i++)
        intervalArray[i] = 0;
    for (j = 0; j < numOfProcesses * MESSAGE_COUNT_PER_PROCESS; j++)
    {
        for (i = 0; i < attr.mq_msgsize; i++)
            receivedArray[i] = 0;
        if ((mqdes = mq_open(DEFAULT_NAME, O_RDWR, 0666, &attr)) < 0)
        {
            perror("open()");
            exit(0);
        }
        if (mq_receive(mqdes, (char *)receivedArray, (dividedLength + arrayLength % MESSAGE_COUNT_PER_PROCESS) * 4, &prio) != -1)
        {
            if (errno == EAGAIN)
            {
                j--;
                mq_close(mqdes);
                // printf("test2 : %d\n",i);
                continue;
            }
            mq_getattr(mqdes, &attr);
            if (MESSAGE_COUNT_PER_PROCESS == 5)
                for (i = 0; i < dividedLength + (prio == 4) * (arrayLength % MESSAGE_COUNT_PER_PROCESS); i++)
                {
                    intervalArray[prio * dividedLength + i] += receivedArray[i];
                    sum[prio] += receivedArray[i];
                    printf("prio : %d : %d\n", prio, receivedArray[i]);
                }
            else
                for (i = 0; i < arrayLength; i++)
                {
                    intervalArray[i] += receivedArray[i];
                    sum[prio] += receivedArray[i];
                    printf("prio : %d : %d\n", prio, receivedArray[i]);
                }
            printf("sum[%d] : %d\n", prio, sum[prio]);
        }
        mq_close(mqdes);
    }
    // printf("test2 : %d\n", j);
    mq_unlink(DEFAULT_NAME);
}
*/