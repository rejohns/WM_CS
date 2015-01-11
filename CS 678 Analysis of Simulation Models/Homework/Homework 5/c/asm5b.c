// asm5b.c

#include <stdio.h>

int main (void) {
	
	int m = 7;
	int a;
	int next;
	int period;
		
	printf("m = %d\n\n", m);
	
	for(a = 1; a < 7; a++) {
		
		printf("a = %d\n\t", a);
		
		period = 0;
		next = a;
		
		printf("%d, ", a);
		
		while (1) {
		
			next = (a * next) % m;
			if(next != a) {
				printf("%d, ", next);
				period++;
			}
			else {
				printf("\e[7m%d\e[0m\n", next);
				break;
			}
		}
		
		printf("\tperiod = %d\n", period);
		
	}
	
	return 0;
	
}