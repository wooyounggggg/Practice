#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>

void sigAlarmHandler(int signo)
{
    printf("SIGINT발생\n");
}
int main()
{
/* timer set*/
    int which = ITIMER_REAL;
    int result;
    struct itimerval value;
    struct sigaction sact;
    memset(&sact, 0, sizeof(sact));
    sact.sa_handler = sigAlarmHandler;
    sigaction(SIGALRM, &sact, NULL);
    value.it_interval.tv_sec = 1;
    value.it_interval.tv_usec = 0;
    value.it_value.tv_sec = 2;
    value.it_value.tv_usec = 0;
    /* timer set*/
    printf("hello world!\n");
    result = setitimer(which, &value, NULL);
    pause();
    printf("Interupt\n");
}
