/* 
 * om.c		Robert Johns		April 29, 2014
 * This program generates random variates (between 0 and 9999) by the midsquare
 * method in its orignal description by Jon von Neumann in 1949 (or Brother Edvin,
 * depeding on your definition of "original"). Prints count and variate to track
 * divergence.
 */

#include <stdio.h>
#include <stdlib.h>

int main (int argc, char * argv[]) {
	
	long count;
	long seed;
	
	if (argc != 2) {
		printf("usage: om [seed]\n");
		exit(-1);
	}
	
	seed = atoi(argv[1]);
	
	if (seed < 1 || seed > 9999) {
		printf("error: seed must be between 1 and 9999\n");
		exit(-1);
	}
	
	count = 1;
	printf ("%4ld \t %ld\n", count, seed);
	count++;
	while (count < 100) {
		
		seed = (seed * seed) / 100 % 10000;
		printf ("%4ld \t %ld\n", count, seed);
		count++;
		
	}
	
	return 0;
	
}