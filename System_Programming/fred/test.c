#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void)
{
    pid_t pid;

    if (fork() == 0)
    {
        if (fork() == 0)
            printf("child\n");
    }
    else
    {
        if (fork() == 0)
            printf("child\n");
    }

    return 0;
}