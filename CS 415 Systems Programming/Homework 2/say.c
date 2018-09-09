//TODO: comments
/*
 * say.c		Robert Johns		February 10, 2014
 * this program takes input from the output of the grep command line utility
 * and outputs the complete fortunes the outputted lines came from.
 * memory is dynamically allocated through one linked list.
 * exits for some bad use, produces garbage output for others.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 // node structure for linked list
struct node {
	char fileName[1023];
	char lineNumber[1023];
	char lineSaying[1023];
	int flag;
	long startNo;
	struct node *next;
	struct node *prev;
};

 // prints a line in hilight mode
void hilightPrint(char string[], char search[]) {
	
	long i;
	for (i = 0; i < strlen(string); i++) {
		if (string[i] != search[0])
			printf("%c", string[i]);
		else {
			long j = 0;
			long away = 0;
			long stop = 0;
			while (away == 0) {
				if (j == strlen(search)) {
					stop = 1;
				}
				if(string[i + j] != search[j]) {
					if (stop != 1) {
						printf("%c", string[i]);
					}
					away = 1;
				}
				if (j == strlen(search)) {
					printf("\e[7m");
					long k;
					for(k = 0; k < strlen(search); k++){
						printf("%c", string[i + k]);
					}
					printf("\e[0m");
					i += strlen(search) - 1;
				}
				j++;
			}
		}
	}
}
 
 // main program
int main(int argc, char *argv[]) {
	
	 // node pointers
	struct node *current;
	struct node *head = NULL;
	struct node *tail = NULL;
	struct node *ptr2 = NULL;
	struct node *ptr = NULL;
	
	 // flags
	int hilight = 0;
	int banner = 0;
	int count;
	
	 // other variables
	FILE *myFile;
	char linePtr[1023];
	char buffer[1023];
	long sayings = 0;
	long nodeNum = 0;
	long i;
	long j;
	long lineNo;
	long counterriffic;
	long start;
	long currLine;
	
	 // sets flags or exits for three argument command
	if (argc == 3) {
		if (strcmp(argv[1], "-h") == 0)
			hilight = 1;
		else if (strcmp(argv[1], "-b") == 0)
			banner = 1;	
		else {
			printf("usage: %s [-b] [-h] pattern\n", argv[0]);
			exit(-1);
		}
		count = 2;
	}
	
	 // sets flags or exits for four argument command
	else if (argc == 4) {
		if (strcmp(argv[1], "-h") == 0 || strcmp(argv[2], "-h") == 0)
			hilight = 1;
		if (strcmp(argv[1], "-b") == 0 || strcmp(argv[2], "-b") == 0)
			banner = 1;
		if ((strcmp(argv[1], "-b") != 0 && strcmp(argv[1], "-h") != 0)
		    || (strcmp(argv[2], "-b") != 0 && strcmp(argv[2], "-h") != 0)) {
			printf("usage: %s [-b] [-h] pattern\n", argv[0]);
			exit(-1);
		}
		count = 3;
	}
	
	 // exits for invalid command lengths
	if (argc != 2 && argc != 3 && argc != 4) {
		printf("usage: %s [-b] [-h] pattern\n", argv[0]);
		exit(-1);
	}
	
	 // store input from grep into linked list
	while (fgets(linePtr, 1023, stdin) != NULL) {
		current = (struct node *)malloc(sizeof(struct node));
		if (current == NULL) {
			printf("error: malloc failure\n");
			exit(-1);
		}
		i = 0;
		j = 0;
		
		while (linePtr[i] != ':') {
			current -> fileName[i] = linePtr[i];
			i++;
		}
		
		i++;
		
		while (linePtr[i] != ':') {
			current -> lineNumber[j] = linePtr[i];
			i++;
			j++;
		}
		
		if(nodeNum == 0) {
			head = current;
			tail = current;
			ptr = current;
			nodeNum = 1;
		}
		else {
			tail -> next = current;
			ptr = tail;
			tail = current;
			tail -> prev = ptr;
		}
		
	}
	
	 // reset pointer
	ptr = head;
	
	 // find starting lines for each saying
	while(ptr != NULL) {
		
		myFile = fopen(ptr -> fileName, "r");
		
		if(myFile == NULL) {
			printf("error: no such file (what? how?)\n");
			exit(-1);
		}
		
		lineNo = atoll(ptr -> lineNumber);
		counterriffic = 0;
		start = 0;
		
		while(counterriffic < lineNo) {
			int yea = strcmp(fgets(buffer, 1023, myFile), "%\n");
			if(yea == 0) {
				start = counterriffic + 2;
			}
			counterriffic++;
		}
		
		ptr -> startNo = start;
		
		ptr = ptr -> next;
		sayings++;
		
		fclose(myFile);
	
	}
	
	 // reset pointer
	ptr = head;
	
	 // remove redundant sayings
	while(ptr != NULL) {
		ptr2 = ptr -> next;
		while(ptr2 != NULL) {
			if(strcmp(ptr -> fileName, ptr2 -> fileName) == 0 &&
			 ptr -> startNo == ptr2 -> startNo) {
				ptr2 -> prev -> next = ptr2 -> next;
				if (ptr2 -> next != NULL)
					ptr2 -> next -> prev = ptr2 -> prev;
				sayings --;
			}
			ptr2 = ptr2 -> next;
		}
		ptr = ptr -> next;
	}
	
	 // print banner if flagged
	if(banner == 1) {
		printf("\n%ld witty sayings contain the string %s\n\n", sayings, argv[count]);
		if (sayings == 0)
			exit(0);
	}
	
	 // reset pointer
	ptr = head;
	
	 // print fortunes
	while (ptr != NULL) {
		
		myFile = fopen(ptr -> fileName, "r");
		
		if(myFile == NULL) {
			printf("error: no such file (what? how?)\n");
			exit(-1);
		}
		
		currLine = 0;
		
		while (currLine < ptr -> startNo - 1 && fgets(buffer, 1023, myFile) != NULL) {		
			currLine++;
		}

		fgets(buffer, 1023, myFile);
		while(strcmp(buffer, "%\n") != 0) {
		
			if (hilight != 0)
				hilightPrint(buffer, argv[count]);
			
			else
				printf("%s", buffer);
			
			fgets(buffer, 1023, myFile);
		}
		
		sayings --;
		if(sayings != 0)
			printf("-------------------------------\n");
		else
			printf("\n");
		
		ptr = ptr -> next;
		
		fclose(myFile);
		
	}
	
	return 0;
	
}