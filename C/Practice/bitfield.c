#include <stdio.h>

struct bitField{
	unsigned A:3;
	unsigned B:1;
};
int main(void)
{
	struct bitField CK;
	CK.A = 5;
	CK.B = 1;
	printf("%d\n",CK);

	return 0;
}
