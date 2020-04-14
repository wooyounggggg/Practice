#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#define MAX_NICE_INDEX 5
#define SUCCESS 1
#define FAIL 0
#define CAUGHT 1
#define UNCAUGHT 2
typedef struct ProcessStructure
{
    struct ProcessStructure *next;
    struct ProcessStructure *prev;
    double vruntime;
    int niceIndex;
    pid_t pid;
} PS;