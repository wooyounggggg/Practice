#include <stdio.h>

int main(void)
{
	int i=23;
	float f = 1.2345;
	FILE *fp;

	fp = fopen("sample.txt","w");

	if(fp!=NULL)
		fprintf(fp,"%10d %16.3f",i,f);
	fclose(fp);
	return 0;
}
