#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	float buffer[1000];
	FILE *fp = NULL;
	size_t size;


	fp=fopen("binary.txt","rb");	
	if(fp==NULL)
	{
		fprintf(stderr,"binary.txt 파일을 열 수 없습니다.\n");
		exit(1);
	}
	size=fread(&buffer,sizeof(float),1000,fp);
	fclose(fp);
	return 0;
}
