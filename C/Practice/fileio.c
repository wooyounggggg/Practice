#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char s[20];
	char save[20];
	FILE *fp;

	printf("파일에 입력할 문자 : ");
	scanf("%s",s);

	fp = fopen("fileIO.txt","w");
	if(fp==NULL)
	{
		fprintf(stderr,"파일 fileIO.txt를 열 수 없습니다.\n");
		exit(1);
	}

	fputs(s,fp);
	fclose(fp);

	fp = fopen("fileIO.txt","r");
	if(fp==NULL)
	{
		fprintf(stderr,"파일 fileIO.txt를 열 수 없습니다.\n");
		exit(1);
	}

	fgets(save,20,fp);
	printf("%s\n",save);
	fclose(fp);
	return 0;
}
