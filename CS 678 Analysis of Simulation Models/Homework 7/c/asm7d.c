#include <stdio.h>
#include <stdlib.h>

int sort (const void * x, const void * y) {
	double xx = *(double*)x, yy = *(double*)y;
	if (xx < yy)
		return -1;
	if (xx > yy)
		return  1;
	return 0;
}

int main(void) {

	double n1 = 6;
	double n2 = 7;
	double n3 = 5;
	double n = n1 + n2 + n3;
	double k = 3;
	double l1;
	double l2;
	double l3;
	double l4;

	double u[] = {
	 1, 7, 49, 38, 22, 32, 41, 43, 57, 33,
	 48, 31, 34, 55, 19, 11, 16, 51, 52, 59,
	 47, 24, 46, 17, 58, 40, 36, 8, 56, 26,
	 60, 54, 12, 23, 39, 29, 20, 18, 4, 28,
	 13, 30, 27, 6, 42, 50, 45, 10, 9, 2,
	 14, 37, 15, 44, 3, 21, 25, 53, 5, 35
	};

	int i;
	for (i = 0; i < 60; i++) {
		u[i] = u[i] / 61.0;
	}

	qsort(u, 60, sizeof(double), sort);

	double t[] = {
	 0.0, 18.0, 38.0, 47.0, 69.0, 117.0, 121.0,
	 127.0, 131.0, 137.0, 139.0, 141.0, 143.0, 147.0, 147.0,
	 157.0, 168.0, 197.0, 237.0, 240.0
	};

	double y[20];
	double v[60];
	
	for(i = 0; i < 20; i++) {
		y[i] = ((i + 1) * n) / ((n + 1) * k * 6.315789);
	}
	
	int j;
	for (i = 0; i < 60; i++) {
		j = 0;
		while(u[i] > y[j]) {
			j++;
		}
		j--;
		l1 = y[j+1] - y[j];
		l2 = t[j+1] - t[j];
		l3 = u[i] - y[j];
		l4 = (l2 * l3) / l1;
		v[i] = t[j] + l4;
	}
	for(i = 0; i < 60; i++) {
		printf("%.3lf, ", v[i]);
		if((i+1) % 10 == 0) printf("\n");
	}
	printf("\n");
}