#include <stdio.h>

int main(void)
{
	int i;
	float f;
	FILE *fp;

	fp = fopen("sample.txt","r");

	if(fp!=NULL)
		fscanf(fp,"%d %f",&i,&f);
	printf("%d %f\n",i,f);
	fclose(fp);
	return 0;
}
