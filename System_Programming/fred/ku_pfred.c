#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX_SIZE 10000
int main(int argc, char *argv[])
{

    int processNum;
    int interval;
    int* numOfRange;
    if(argc == 1)
    {
        printf("Error : There's no input!\n");
        exit(1);
    }
    FILE* fp;
    processNum = *argv[1]-'0';
    interval = *argv[2]-'0';
    numOfRange = (int*)malloc(sizeof(int) * MAX_SIZE / interval);
    const char* fileName = argv[3];
    if((fp = fopen(fileName, "r")) == NULL)
    {
        printf("file open failed\n");
        exit(1);
    }
    fclose(fp);    
    return 0;
}
