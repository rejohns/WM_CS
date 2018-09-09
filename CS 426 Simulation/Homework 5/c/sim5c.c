#include <stdio.h>
#include "rvms.h"
#include <math.h>

int main(void) {
	
	double n = cdfBinomial(100, .1, 16) - cdfBinomial(100, .1, 3);
	double alpha = 1/n;
	double sum = 0;
	double sum2 = 0;
	double index;
	double mean;
	double variance;
	double std; 
	
	for (index = 4; index <= 16; index++) {
		sum += (index * pdfBinomial(100, .1, index));
		sum2 += (index * index * pdfBinomial(100, .1, index));
	}
	
	mean = sum * alpha;
	
	printf("mean: %5.4f\n", mean);
	
	variance = sum2 * alpha - mean * mean;
	
	printf("variance: %5.4f\n", variance);
	
	std = sqrt(variance);
	
	printf("standard d: %5.4f\n", std);
	
	return 0;

}