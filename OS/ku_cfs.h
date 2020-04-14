#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#define MAX_NICE_INDEX 5
#define SUCCESS 1
#define FAIL 0
typedef struct ProcessStructure
{
    struct ProcessStructure *next;
    struct ProcessStructure *prev;
    double vruntime;
    int niceIndex;
    pid_t pid;
} PS;