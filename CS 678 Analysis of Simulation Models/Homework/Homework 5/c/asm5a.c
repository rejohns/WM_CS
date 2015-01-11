#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rngs.h"
#include "rvgs.h"

static double chi25 = 9.590777;
static double chi975 = 34.16961;
static double norm975 = 1.959964;
static double crit1 = 1.358;
static double crit3 = 1.094;
static double MAX = 10000;
double countEx = 0;
double countAp = 0;
double ksTAcc = 0;
double ksFAcc = 0;

int sort (const void * x, const void * y) {
	double xx = *(double*)x, yy = *(double*)y;
	if (xx < yy) return -1;
	if (xx > yy) return  1;
	return 0;
}

double run(void) {
	
	double variates[10];
	double temp;
	double sum = 0;
	double mean;
	double lowEx;
	double highEx;
	double lowAp;
	double highAp;
	double dnPlus;
	double dnMinus;
	double dn = -1000.0;
	double atsT;
	double atsF;
	
	/* Generate variates */
	
	printf("(a) Variates:\n\t");
	
	int i;
	for(i = 0; i < 10; i++) {
		temp = Exponential(1.0);
		printf("%.4lf, ", temp);
		variates[i] = temp;
		sum += temp;
	}
	
	mean = sum / 10.0;
	
	/* Compute exact confidence interval */
	
	printf("\n(b) Exact confidence interval:\n");
	
	lowEx  = (2.0 * 10.0 * mean) / chi975;
	highEx = (2.0 * 10.0 * mean) / chi25;
	
	if (lowEx <= 1.0 && 1.0 <= highEx) {
		countEx += 1.0;
	}
	
	printf("\tPr(%lf < mean < %lf) = .95\n", lowEx, highEx);
	
	/* Compute approximate confidence interval */
	
	printf("(c) Approximate confidence interval:\n");
	
	lowAp  = mean - norm975 * sqrt(((- 10.0)/ (10.0 - 2.0 * 10.0)) / 10.0);
	highAp = mean + norm975 * sqrt(((- 10.0)/ (10.0 - 2.0 * 10.0)) / 10.0);
	
	if (lowAp <= 1.0 && 1.0 <= highAp) {
		countAp += 1.0;
	}
	
	printf("\tPr(%lf < mean < %lf) = .95\n", lowAp, highAp);
	
	/* Run K-S test for theoretical distribution */
	
	printf("(d) K-S test results with theoretical distribution:\n");
	
	qsort(variates, 10, sizeof(double), sort);
	
	int j;
	for (j = 0; j < 10; j++) {
	
		dnPlus  =  (((double) j) / 10.0) - (1.0 - exp(-1.0 * variates[j]));
		dnMinus =  (1.0 - exp(-1.0 * variates[j])) - (((double) j - 1.0) / 10.0);
		if (dnPlus > dn)
			dn = dnPlus;
		if (dnMinus > dn)
			dn = dnMinus;
	}
	
	atsT = (sqrt(10.0) + .12 + 0.11/sqrt(10.0)) * dn;
	
	if (atsT<= crit1) {
		ksTAcc += 1;
		printf("\tats %lf <= crit %lf -- accept null hypothesis\n", atsT, crit1);
	}
	else
		printf("\tats %lf > crit %lf -- reject null hypothesis\n", atsT, crit1);	
		
	/* Run K-S test results with fitted distribution */
	
	printf("(e) K-S test results with fitted distribution:\n");
	
	dn = -1000.0;
	
	int k;
	for(k = 0; k < 10; k++) {
		
		dnPlus  =  (((double) k) / 10.0) - (1.0 - exp((-1.0 * variates[k])/ mean));
		dnMinus =  (1.0 - exp((-1.0 * variates[k])/ mean)) - (((double) k - 1.0) / 10.0);
		if (dnPlus > dn)
			dn = dnPlus;
		if (dnMinus > dn)
			dn = dnMinus;
	}
	
	atsF = (dn - .2/10.0) * (sqrt(10.0) + .26 + .5 / sqrt(10.0));
	
	if (atsF<= crit3) {
		ksFAcc += 1;
		printf("\tats %lf <= crit %lf -- accept null hypothesis\n", atsF, crit3);
	}
	else
		printf("\tats %lf > crit %lf -- reject null hypothesis\n", atsF, crit3);
	
	return 0;
	
}

int main(void) {
	
	SelectStream(0);
	PlantSeeds(123456789);
	
	printf("Simulating 10,000 times...\n\n");
	
	int i;
	for(i = 0; i < MAX; i++) {
		run();
		printf("\n");
	}
	
	printf("(f) Summary:\n");
	
	printf("\tPercentage of exact confidence intervals that contain 1.0:\n\t\t%.4lf\n", 
	 countEx / MAX);
	
	printf("\tPercentage of approximate confidence intervals that contain 1.0:\n\t\t%.4lf\n", 
	 countAp / MAX);
	 
	printf("\tPercentage of accepted hypotheses with theoretical distribution:\n\t\t%.4lf\n", 
	 ksTAcc / MAX);
	 
	printf("\tPercentage of accepted hypotheses with fitted distribution:\n\t\t%.4lf\n", 
	 ksFAcc / MAX);
	
	return 0;
	
}