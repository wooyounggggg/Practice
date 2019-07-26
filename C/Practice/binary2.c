#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	FILE *fp = fopen("binarysample.txt","wb");
	int buffer[20];
	int buffer_2[20];
	int i;
	if(fp==NULL)
	{
		fprintf(stderr,"binarysample 파일을 열 수 없습니다\n");
		exit(1);
	}
	for(i=0;i<20;i++)
		buffer[i] = i;
	fwrite(&buffer,sizeof(buffer[0]),sizeof(buffer)/sizeof(buffer[0]),fp);
	fclose(fp);
	fp = fopen("binarysample.txt","rb");
	fread(&buffer_2,sizeof(buffer_2[0]),sizeof(buffer_2)/sizeof(buffer_2[0]),fp);
	for(i=0;i<20;i++)
		printf("%d\n",buffer_2[i]);

	return 0;
}
