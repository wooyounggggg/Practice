#include <stdio.h>
#include <string.h>

int main(void)
{
	char ch;
	while(ch!='q')	
	{
		ch = getchar();
		putchar(ch);
	}
	return 0;
}
