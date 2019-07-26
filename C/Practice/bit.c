#include <stdio.h>

int main(void)
{
	int input;
	int i;

	input = 3;
	for(i=0;i<32;i++)
	{
		if(0x80000000&input<<i)
			printf("1");
		else printf("0");
	}
	return 0;
}
