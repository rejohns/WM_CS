#include<stdio.h>
#include<math.h>

int gcd (long a, long b) {
	int c;
	while ( a != 0 ) {
		c = a; a = b%a;  b = c;
	}
	return b;
}

long g(long x) {
	long t;
	long a = 48271;
	long m = pow(2, 31) - 1;
	long q = m / a;
	long r = m % a;
	
	t = a * (x % q) - r * (x / q);
	if (t > 0) {
		return(t);
	}
	else {
		return(t + m);
	}
}

int main(void) {

	long i = 1;
	long a = 7;
	long x = a;
	long count = 0;
	long m = pow(2,31) - 1;

	while(x != 1 && count < 10) {
		if (gcd(i, m -1) == 1) {
			printf("x = %d, i = %d\n", x, i);
			count++;
		}
		i++;
		x = (a*x)%m;
	}

	return 0;
}