/* driver.c		Robert Johns	February 24, 2014
 * This program is the ultimate ancestor for a suite of three programs which parse a
 * file into characters and digits only, and prints out a count of each string.
 * This task is accomplished using pipes. driver.c creates these pipes, and then
 * forks, dups and execs appropriately to create the system.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>

 // pipe arrays
int s2e[2];
int e2s[2];
int s2o[2];
int o2s[2];

 // pipe file descriptors
#define S2E_0 16
#define S2E_1 17
#define E2S_0 18
#define E2S_1 19
#define S2O_0 20
#define S2O_1 21
#define O2S_0 22
#define O2S_1 23

int main (int argc, char * argv[]) {

	int file;
	pid_t scanFork;
	pid_t evenFork;
	pid_t oddFork;
	int execlCheck;
	
	if (argc != 2) {
		printf("usage: %s [file]\n", argv[0]);
		exit(-1);
	}
	
	file = open(argv[1], O_RDONLY);
	
	if (file == -1) {
		printf("error opening file\n");
		exit(-1);
	}
	
	 // duplicate file for use by other processes
	dup2(file, 0);
	
	 // create pipes
	pipe(s2e);
	pipe(e2s);
	pipe(s2o);
	pipe(o2s);
	
	 // fork scanner into existence
	scanFork = fork();
	
	if(scanFork == -1) {
		printf("error forking scanner\n");
		exit(-1);
	}
	
	 // do scanner stuff
	else if(scanFork == 0) {
		
		 // close unneeded pipes
		close(s2e[0]);
		close(e2s[1]);
		close(s2o[0]);
		close(o2s[1]);
		
		 // dupe needed pipes
		dup2(s2e[1], S2E_1);
		dup2(e2s[0], E2S_0);
		dup2(s2o[1], S2O_1);
		dup2(o2s[0], O2S_0);
		
		 // close remaining pipes
		close(s2e[1]);
		close(e2s[0]);
		close(s2o[1]);
		close(o2s[0]);
		
		 // run scanner.c
		execlCheck = execl("./scanner", "scanner", NULL);
		
		if (execlCheck == -1) {
			printf("error using execl on scanner\n");
			exit(-1);
		}
	}
	
	else {
		 // fork even into existence
		evenFork = fork();
			
		if (evenFork == -1) {
			printf("error forking even\n");
			exit(-1);
		}
		
		// do even stuff
		else if (evenFork == 0) {
			
			 // close unneeded pipes
			close(s2e[1]);
			close(e2s[0]);
			close(s2o[0]);
			close(s2o[1]);
			close(o2s[0]);
			close(o2s[1]);
			
			 // dupe needed pipes
			dup2(s2e[0], S2E_0);
			dup2(e2s[1], E2S_1);
			
			 // close remaining pipes
			close(s2e[0]);
			close(e2s[1]);
			
			 // run evenodd.c as even
			execlCheck = execl("./evenodd", "even", "1", NULL);
			
			if (execlCheck == -1) {
				printf("error using execl on even\n");
				exit(-1);
			}
		}
		
		 // fork odd into existence
		else {
			oddFork = fork();
			if (oddFork == -1) {
				printf("error forking odd\n");
				exit(-1);
			}
			
			// do odd stuff
			else if (oddFork == 0) {
				
				 // close unneeded pipes
				close(s2e[0]);
				close(s2e[1]);
				close(e2s[0]);
				close(e2s[1]);
				close(s2o[1]);
				close(o2s[0]);
				
				 // dup needed pipes
				dup2(s2o[0], S2O_0);
				dup2(o2s[1], O2S_1);
				
				 // close remaining pipes
				close(s2o[0]);
				close(o2s[1]);
				
				 // run evenodd.c as odd
				execlCheck = execl("./evenodd", "odd", "1", NULL);
				
				if (execlCheck == -1) {
					printf("error using execl on odd\n");
					exit(-1);
				}
			}
		}
	}
	
	 // close unneeded pipes
	close(s2e[0]);
	close(s2e[1]);
	close(e2s[0]);
	close(e2s[1]);
	close(s2o[0]);
	close(s2o[1]);
	close(o2s[0]);
	close(o2s[1]);
	
	 // wait for scanner to wrap up and terminate
	waitpid(scanFork, NULL, 0);
	
	return 0;
	
}