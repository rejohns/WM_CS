/* beetle.c		author: Robert Johns		date: 1/25/14
 * Determines the mean survival time of a user-specified number of beetles suspended over 
 * a vat of boiling oil when dropped in the center of a user-specified size square board.
 * Exits for bogus input including negative numbers, decimals and strings.
 */

#include <stdio.h>		//printf
#include <stdlib.h>		//exit, atol
#include <math.h>		//M_PI, sin, cos
#include <string.h>		//strlen
#include <ctype.h>		//isdigit
#include <limits.h>		//LONG_MAX

int main(int argc, char *argv[]) {

	if (argc != 3) {
		printf("usage: %s [squares] [beetles]\n", argv[0]);
		exit(-1);
	}
	
	int i;
	for (i = 0; i < strlen(argv[1]); i++) {
		if(isdigit(argv[1][i]) == 0) {
			printf("error: board dimension must be an integer\n");
			exit(-1);
		}
	}
	
	int j;
	for (j = 0; j < (strlen(argv[2])); j++) {
		if(isdigit(argv[2][j]) == 0) {
			printf("error: number of tests must be an integer\n");
			exit(-1);
		}
	}
		
	long board = atol(argv[1]);
	long beetles = atol(argv[2]);
	
	if (board + 1 < 0) {
		printf("error: board must have reasonable dimensions\n");
		exit(-1);
	}
	
	if (beetles + 1 < 0) {
		printf("error: must have reasonable number of tests\n");
		exit(-1);
	}
				
	long runum = 0;
	long long count;
	double xpos;
	double ypos;
	double randPol;
	
	long run;
	for(run = 0; run < beetles; run++){
	
		count = 0;
	
		xpos = (double) board / 2;
		ypos = (double) board / 2;
	
		while(xpos >= 0 && ypos >= 0 && xpos <= (double)board && ypos <= (double)board){
		
			randPol = 2 * M_PI * (random() / (double) RAND_MAX);
	
			xpos += cos(randPol);
			ypos += sin(randPol);
			count++;
			
			if(count < 0) {
				printf("error: overflow in beetle lifetime\n");
				exit(-1);
			}
			
			if(xpos >= 0 && ypos >= 0 && xpos <= (double)board && ypos <= (double)beetles)
				count++;
				if(count < 0) {
					printf("error: overflow in beetle lifetime\n");
					exit(-1);
				}
			
		}
		
		runum += count;
		
		if(runum < 0) {
			printf("error: overflow in moving average\n");
			exit(-1);
		}
		
	}
	
	printf("%ld by %ld square, %ld beetles, mean beetle lifetime is %.1lf\n",
	  board, board, beetles, (double) runum / (double) beetles);

	exit(0);
	
}