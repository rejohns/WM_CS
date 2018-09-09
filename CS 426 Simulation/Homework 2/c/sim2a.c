#include<stdio.h>
#include<math.h>

int main(void) {

	long p = 1;
	long a = 447489615;
	long x = a;
	long m = pow(2,31) - 1;

	while (x != 1) {
		p++;
		x = (a*x)%m;
	}
	if (p == m - 1) {
		printf("yea\n");
	}
	else {
		printf("nay\n");
	}
	return 0;
}