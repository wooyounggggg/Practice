#include <stdio.h>
#include <time.h>
#include <windows.h>

int main(void)
{
	time_t timer;
	struct tm *t;
	
	timer = time(NULL);



	//printf("current year : %d\n", t->tm_year+1900);
	t = localtime(&timer);
	printf("%d\n",t->tm_hour);
	printf("%d\n",timer);
	timer += 60*60;
	printf("%d\n",timer);
	t = localtime(&timer);
	printf("%d\n",t->tm_hour);


	return 0;
}
