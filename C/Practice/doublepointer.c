#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int i,j;
	int **p;
	p = (int**)malloc(5*sizeof(int*));
	for(i=0;i<5;i++)
		*(p+i) = (int*)malloc(i*sizeof(int));	
	
	for(i=0;i<5;i++)
		for(j=0;j<6;j++)
		{
			*(*(p+i)+j) = i*j;
			printf("%d\n",*(*(p+i)+j));
		}
	return 0;
}
