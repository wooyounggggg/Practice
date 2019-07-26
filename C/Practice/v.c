#include <stdio.h>

int main(void)
{
	int i,j;

	for(i=9;i>=0;i--)
	{
		for(j=0;j<9-i;j++)
			printf("  ");
				
		for(j=0;j<i;j++)
			printf("%d ",j);


		for(j=i;j>=0;j--)
			printf("%d ",j);

		printf("\n");	
	}
}
