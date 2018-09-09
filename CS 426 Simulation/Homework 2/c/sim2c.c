#include<stdio.h>
#include<math.h>

int gcd (long a, long b) {
	int c;
	while ( a != 0 ) {
		c = a; a = b%a;  b = c;
	}
	return b;
}

int main(void) {

	long i = 1;
	long a = 7;
	long x = a;
	long m = pow(2,31) - 1;

	while(x != 1) {
		if (gcd(i, m -1) == 1 && x == 48271) {
			printf("x = %d, i = %d\n", x, i);
			return 0;
		}
		i++;
		x = (a*x)%m;
	}

	return 0;
}