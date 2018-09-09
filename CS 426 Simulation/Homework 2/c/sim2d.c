#include<stdio.h>
#include "rng.c"

int main(void) {

	int pos[] = {0,0,0,0,0,0,0,0,0,0,0,0};
	double count = 0;
	while(count < 100000000) {
		double tr1 = Random() * 2;
		int r1 = floor(tr1);
		if (r1 == 0) {
			int tr2 = Random() * 6;
			int r2 = floor(tr2);
			pos[r2]++;
		}
		else {
			double tr3 = Random() * 6;
			int r3 = floor(tr3) + 1;
			double tr4 = Random() * 6;
			int r4 = floor(tr4) + 1;
			int sum = r3 + r4;
			pos[sum-1]++;
		}
		count++;
	}
	int j;
	for(j = 0; j < 12; j++) {
		double prob = pos[j]/count;
		printf("Probability of %d: %.5f\n", j+1, prob);
	}
	return 0;
}	