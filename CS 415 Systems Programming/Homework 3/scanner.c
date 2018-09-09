/* scanner.c	Robert Johns	February 24, 2014
 * This program, a fork of driver, takes a file as input and parses the data to remove
 * punctuation and whitespace.  It then pipes the content to even and odd, depending on
 * whether the word has even or odd characters. Upon even and odd's return, it prints
 * an asterisk every second until even signals, and then prints the contents.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>

 // universal pipe file descriptors
#define S2E_0 16
#define S2E_1 17
#define E2S_0 18
#define E2S_1 19
#define S2O_0 20
#define S2O_1 21
#define O2S_0 22
#define O2S_1 23

 // signal flag and handler
int sig;
void handle() {
	sig++;
}

int main (int argc, char *argv[]) {
	
	pid_t myPid = getpid();
	char temp[1023];
	char final[1023];
	char readIn1[1023];
	char readIn2[1023];
	int i;
	int y = 0;
	sig = 0;
	
	 // write pid to even
	sprintf(temp, "%d", myPid);
	write(S2E_1, temp, sizeof(temp));
	
	 // parse input
	while (scanf("%s", temp) != EOF) {
		y = 0;
		memset(&final[0], 0, sizeof(final));
		
		for(i = 0; i < strlen(temp); i++) {
			if (ispunct(temp[i]) || iscntrl(temp[i]) || isblank(temp[i])) {
				continue;
			}
			else if (isupper(temp[i])) {
				final[y] = tolower(temp[i]);
				y++;
				continue;
			}
			else {
				final[y] = temp[i];
				y++;
			}
		}
		
		 // pipe to even or odd
		if (y > 0) {
			if (y % 2 == 0)
				write(S2E_1, &final, sizeof(final));
			else
				write(S2O_1, &final, sizeof(final));
		}	
	}
	
	if (close(S2E_1) == -1) {
		printf("error closing pipe s2e[1]\n");
		exit(-1);
	}
	if(close(S2O_1) == -1) {
		printf("error closing pipe s20[1]\n");
		exit(-1);
	}
	
	 // wait for signal
	signal(SIGTERM, handle);
	
	while(sig == 0) {
		sleep(1);
		write(1, "*", sizeof("*"));
	}
	
	 // print output
	printf("\nWords with even letters:\n");
	while(read(E2S_0, &readIn1, sizeof(readIn1)) > 0) {
		read(E2S_0, &readIn2, sizeof(readIn2));
		printf("\t%-20s%-10s\n", readIn1, readIn2);
	}
	
	printf("Words with odd letters:\n");
	while(read(O2S_0, &readIn1, sizeof(readIn1)) > 0) {
		read(O2S_0, &readIn2, sizeof(readIn2));
		printf("\t%-20s%-10s\n", readIn1, readIn2);
	}
	
	return 0;
	
}