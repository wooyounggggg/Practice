#include <stdio.h>
#include <time.h>
#include <windows.h>

int main(void)
{
	time_t timer;
	struct tm *t;
	
	timer = time(NULL);



    while(1){
	//printf("current year : %d\n", t->tm_year+1900);
	t = localtime(&timer);
	printf("%d\n",t->tm_sec);
	//printf("%d\n",timer);
    Sleep(1000);
    timer = time(NULL); 
    t = localtime(&timer);
	//printf("%d\n",timer);
	//t = localtime(&timer);
	printf("%d\n",t->tm_sec);
    }


	return 0;
}
