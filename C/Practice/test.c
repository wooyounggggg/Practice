#include <stdio.h>
#include <string.h>

int main(void)
{
	char abc[30] = "Programming Project Michin";
	char *token = " ,.\n";
	char *save;
		
	save = strtok(abc,token);
		printf("%s\n",save);
	while(save=strtok(NULL,token))
	{
		printf("%s\n",save);
	}
	return 0;
}
