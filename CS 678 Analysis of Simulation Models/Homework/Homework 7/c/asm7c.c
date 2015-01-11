#include <stdio.h>

int zee(int z) {
	return (7 * z) % 61;
}

int main(void) {
	
	int z = 1;
	
	printf("%d, ", z);
	
	int i;
	for(i = 0; i < 61; i++) {
		z = zee(z);
		printf("%d, ", z);
	}
	
	return 0;
}