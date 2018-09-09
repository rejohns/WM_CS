#include <stdio.h>
#include <math.h>
#include "rngs.h"

long	r	= 1000;
double	a	= 4.0;
double	b	= 8.0;
double	c	= 7.0;

double pdf(double x) {
	
	if(x >= a && x <= c)
		return (2 * (x - a)) / ((b - a) * (c - a));
	else if (x > c && x <= b)
		return (2 * (b - x)) / ((b - a) * (b - c));
	else	
		return -1;
}

double idf(double u) {
	
	double	u0	= (c - a) / (b - a);
	
	double x;
	
	if(u < u0)
		return sqrt(u * (b - a) * (c - a)) + a;
	else
		return b - sqrt((b - a) * (b - c) * (1 - u));
}

double idf1 (double u) {
	return idf(u);
}

double idf2 (double u) {
	return idf(u);
}

void intri(void) {
	
	double v;
	double mean = 0;
	
	int i;
	for (i = 0; i < r; i++) {
		if(i % 10 == 0)
			printf("\n");
		v = idf(Random());
		mean += v / (double) r;
		printf("%.3lf ", v);
	}
	printf("\nmean from inversion: %lf", mean);
	printf("\n");
	
}

void artri (void) {

	double mode = 2 / (b - a);
	double x;
	double y;
	double mean = 0;
	
	long count = 0;
	while(count < 1000) {
		x = (Random() * 4.0) + 4.0;
		y = Random() * mode;
		if (y <= pdf(x)) {
			printf("%.3lf ", x);
			count ++;
			if (count % 10 == 0) printf("\n");
			mean += x / (double) r;
		}
	}
	printf("mean from acceptance/rejection: %lf\n", mean);
}




void cotri (void) {
	double u;
	double v;
	double mean = 0;
	
	int i;
	for(i = 0; i < 1000; i++) {
		
		if (i % 10 == 0)
			printf("\n");
		
		u = Random();
		
		if (u <= (c - a) / (b - a)) {
			v = idf1(Random());
		} 
		else {
			v = idf2(Random());
		}
		mean += v / (double) r;
		printf("%.3lf ", v);
	}
	printf("\nmean for composition: %lf\n", mean);
}

int main(void) {
	
	PlantSeeds(123456789);
	
	printf("Inverse CDF Variates:");
	intri();
	printf("\nAcceptance/Rejection Variates:\n");
	artri();
	printf("\nComposition Variates:");
	cotri();
	
	return 0;
}