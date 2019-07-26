#include <stdio.h>
#include <stdlib.h>

int add(int,int);
int main(void)
{
	int result;
	int (*pf)(int,int);
	pf = add;
	result = pf(10,20);
	printf("result = %d\n",result);
	return 0;
}

int add(int x,int y)
{
	return x+y;
}
