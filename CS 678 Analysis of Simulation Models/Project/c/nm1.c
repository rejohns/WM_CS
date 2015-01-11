/* 
 * nm1.c		Robert Johns		April 29, 2014
 * This program generates random variates (between 0 and 9999) by the first attempt
 * at an improved midsquare method as described by Robert Johns in the final project
 * for CSCI 678: Statistical Analysis of Simulation Models in the Spring 2014 term
 * at William & Mary.  
 */

#include <stdio.h>
#include <stdlib.h>

int main (int argc, char * argv[]) {
	
	long 	  count;
	long 	  max;
	long long seed;
	long long seq1[] = {7, 3, 2, 5, 4, 8, 9, 0, 1, 5};
	long long seq2[] = {4, 2, 7, 9, 5, 0, 6, 2, 3, 1};
	long long seq3[] = {5, 7, 4, 9, 8, 6, 2, 0, 1, 3};
	long long seq4[] = {1, 7, 9, 5, 2, 3, 2, 8, 4, 6};
	
	if (argc != 3) {
		printf("usage: nm1 [seed] [max]\n");
		exit(-1);
	}
	
	seed = atoll(argv[1]);
	max = atol(argv[2]);
	
	if (seed < 1 || seed > 9999) {
		printf("error: seed must be between 1 and 9999\n");
		exit(-1);
	}
	
	count = 0;
	printf ("%lld\n", seed);
	count++;
	while (count < max) {
		
		seed = seed * 100;
		seed += (10 * seq1[count % 10] + seq2[count % 10]);
		seed += (10000000 * seq3[count % 10] + 1000000 * seq4[count % 10]);
		seed = ((seed * seed) / 1000000) % 10000;
		printf ("%lld\n", seed);
		count++;
		
	}
	
	return 0;
	
}