#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
	long a, b, r;

	a = atoi(argv[1]);
	b = atoi(argv[2]);

	//__asm__ __volatile__("asm",output,input,clobber)
	__asm__ __volatile__(
			"movq	%1, %%rax\n\t"
			"addq	%2, %%rax\n\t"
			"imul	%%rax\n\t" 
			"movq	%%rax, %0\n\t"
			: "=r"(r) //output = r ouput을 나타낼 때 =r 사용
			: "r"(a), "r"(b) //input %1 = a, %2 = b
			);			
	printf("a = %ld, b = %ld, r = %ld \n", a, b, r);
	return 0;
}
