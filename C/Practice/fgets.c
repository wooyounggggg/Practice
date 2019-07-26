#include <stdio.h>

int main(void)
{
	FILE *fp,*fp2;
	char s[20];
	fp = fopen("sample.txt","r");
	fp2 = fopen("copied.txt","w");

	while(fgets(s,20,fp)!=NULL)
		fputs(s,fp2);

	return 0;
}
