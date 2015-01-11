/* evenodd.c	Robert Johns	February 24, 2014
 * This program takes a string of words from scanner.c (or standard input if running
 * as a standalone program) and sorts them, incrementing a counter if words are
 * found twice. It then pipes the data back to scanner for printing, or just
 * prints it itself.
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

 // system-wide file descriptors
#define S2E_0 16
#define S2E_1 17
#define E2S_0 18
#define E2S_1 19
#define S2O_0 20
#define S2O_1 21
#define O2S_0 22
#define O2S_1 23

 // node for storage
struct node {
	char word[1023];
	int count;
	struct node * next;
	struct node * prev;
};

int main(int argc, char *argv[]) {
	
	int even;
	int scan;
	int rfd;
	int wfd;
	struct node *head = NULL;
	struct node *ptr  = NULL;
	struct node *new;
	char readIn[1023];
	char temp[1023];
	int done;
	int scanPid;	
	
	if (argc != 2) {
		printf("usage: %s [pid]\n", argv[0]);
		exit(-1);
	}
	
	if (strcmp("even", argv[0]) == 0 || strcmp("./even", argv[0]) == 0) {
		even = 1;
		rfd = S2E_0;
		wfd = E2S_1;
	}
	else if (strcmp("odd", argv[0]) == 0 || strcmp("./odd", argv[0]) == 0) {
		even = 0;
		rfd = S2O_0;
		wfd = O2S_1;
	}
	else {
		printf("binary may only be invoked as 'even' or 'odd'\n");
		exit(-1);
	}
	
	if (strcmp("1", argv[1]) == 0) {
		scan = 1;
	}
	
	else if (strcmp("0", argv[1]) == 0) {	
		scan = 0;
		rfd = 0;
		wfd = 1;
	}
	
	else {
		printf("invalid pid. 0 for standalone, 1 for scanner\n");
		exit(-1);
	}
	
	 // get pid of scanner for signal if running as even
	if(even && scan) {
		read(rfd, readIn, sizeof(readIn));
		scanPid = atoi (readIn);
	}
	
	 // in scanner mode, create sorted linked list on fly
	if(scan) {
		while (read(rfd, readIn, sizeof(readIn)) != 0) {
			if (getenv("EODELAY") != NULL) {
				if (atoll (getenv("EODELAY")) > 0) {
					sleep (atoll (getenv("EODELAY")));
				}
			}	
			if (head == NULL) {
				new = (struct node *) malloc(sizeof(struct node));
				strcpy(new -> word, readIn);
				new -> count = 0;
				head = new;
			}
			ptr = head;
			done = 0;
			while (done == 0) {
				if (strcmp (readIn, ptr -> word) < 0) {
					new = (struct node *) malloc(sizeof(struct node));
					strcpy(new -> word, readIn);
					new -> count = 1;
					if (ptr -> prev != NULL) {
						new -> next = ptr;
						new -> prev = ptr -> prev;
						new -> prev -> next = new;
						new -> next -> prev = new;
					}
					else {
						new -> next = ptr;
						ptr -> prev = new;
						head = new;
					}
					done = 1;
				}
				else if (strcmp (readIn, ptr -> word) == 0) {
					ptr -> count++;
					done = 1;
				}
				else {
					if (ptr -> next == NULL) {
						new = (struct node *) malloc(sizeof(struct node));
						strcpy(new -> word, readIn);
						new -> count = 1;
						ptr -> next = new;
						new -> prev = ptr;
						done = 1;
					}
					else
						ptr = ptr -> next;
				}
			}
		}
	}
	
	 // in standalone mode, create sorted linked list on fly
	else {
		while (scanf("%1024s", readIn) != EOF) {
			if (getenv("EODELAY") != NULL) {
				if (atoll (getenv("EODELAY")) > 0) {
					sleep (atoll(getenv("EODELAY")));
				}
			}
			if (head == NULL) {
				new = (struct node *) malloc(sizeof(struct node));
				strcpy(new -> word, readIn);
				new -> count = 0;
				head = new;
			}
			ptr = head;
			done = 0;
			while (done == 0) {
				if (strcmp (readIn, ptr -> word) < 0) {
					new = (struct node *) malloc(sizeof(struct node));
					strcpy(new -> word, readIn);
					new -> count = 1;
					if (ptr -> prev != NULL) {
						new -> next = ptr;
						new -> prev = ptr -> prev;
						new -> prev -> next = new;
						new -> next -> prev = new;
					}
					else {
						new -> next = ptr;
						ptr -> prev = new;
						head = new;
					}
					done = 1;
				}
				else if (strcmp (readIn, ptr -> word) == 0) {
					ptr -> count++;
					done = 1;
				}
				else {
					if (ptr -> next == NULL) {
						new = (struct node *) malloc(sizeof(struct node));
						strcpy(new -> word, readIn);
						new -> count = 1;
						ptr -> next = new;
						new -> prev = ptr;
						done = 1;
					}
					else
						ptr = ptr -> next;
				}
			}
		}
	}	
	
	ptr = head;
	
	 // print in scanner mode
	if (scan) {
		if (even) {
			sleep(10);
			kill(scanPid, SIGTERM);
			while (ptr != NULL) {
				write (wfd, &ptr -> word, sizeof(ptr -> word));
				sprintf(temp, "%d", ptr -> count);
				write (wfd, temp, sizeof(temp));
				ptr = ptr -> next;
			}
			close (E2S_1);
		}
		else {
			while (ptr != NULL) {
				write (wfd, &ptr -> word, sizeof(ptr -> word));
				sprintf(temp, "%d", ptr -> count);
				write (wfd, temp, sizeof(temp));
				ptr = ptr -> next;
			}
			close(O2S_1);
		}
	}
	
	 // print in standalone mode
	else {
		while (ptr != NULL) {
			printf("\t%-20s%-5d\n", ptr -> word, ptr -> count);
			ptr = ptr -> next;
		}
	}
	
	return 0;
	
}