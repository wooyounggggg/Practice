#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#define NUM_OF_NICE_GROUP 5
typedef struct ProcessStructure
{
    struct ProcessStructure *next;
    struct ProcessStructure *prev;
    double vruntime;
    double niceValue;
    pid_t pid;
} ProcessStructure;