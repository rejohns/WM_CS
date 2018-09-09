/* ttt.c	Robert Johns	March 21, 2014
 * This program acts as a client for a distributed tic-tac-toe game. It can play a game
 * or query the server for data.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> 
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#define MSGSIZE 33

void wtsock (int sock, char msg[]) {
	int num, left, put;
	left = MSGSIZE; put=0;
  	while (left > 0) {
    	if((num = write(sock, &msg[put], left)) < 0) {
    		perror("inet_wstream:write");
    		exit(-1);
    	}
    	else left -= num;
    	put += num;
  	}
}

void printboard (char board[]) {
	printf(" %c | %c | %c \n", board[1], board[2], board[3]);
	printf("-----------\n");
	printf(" %c | %c | %c \n", board[4], board[5], board[6]);
	printf("-----------\n");
	printf(" %c | %c | %c \n", board[7], board[8], board[9]);
}

int isvalid(char move[], char board[]) {
	int index;
	index = atoi(move);
	if (move[0] == board[index]) {
		return index;
	}
	return 0;
}

int main(int argc, char * argv[]) {

	char msg[MSGSIZE];					// message to send
	char dgmsg[1024];
	char recv[MSGSIZE];					// message received
	int query;							// user-specified query boolean
	int wait;							// user-specified wait boolean
	int timeout;						// user-specified timeout
	int i;								// index for getting timeout
	int count = 0;						// number of bytes received
	int checkDG;						// sendto error variable
	int dg_sender;						// datagram sender socket
	int sock, ecode;					// socket number, socket information
	char hostname[1024];				// host name
	char port[1024];					// port number
	char buff[1024];					// buffer for query
	char * ptr;							// pointer to kill newline
	FILE* file; 						// file to read hostname, port
  	struct sockaddr_in *server;			// server addrinfo
  	struct addrinfo hints, *addrlist;	// addr info
  	socklen_t length_dg;				// length of datagram socket
  	int j;								// indexer
  	int invalid;						// invalid move boolean
  	char player;						// indicator of x or o
  	int index;
  	int time;
  	int notready;
  	fd_set womp;
  	struct timeval usrt;
  	int hits;
	
	if (argc == 1) {
		timeout = 0;
		query = 0;
	}
	
	else if (argc == 2 && strcmp(argv[1], "-q") == 0) {
		query = 1;
		timeout = 0;
	}
	
	else if (argc == 3 && strcmp(argv[1], "-t") == 0) {
		timeout = 1;
		query = 0;
		for (i = 0; i < (strlen(argv[2])); i++) {
			if (isdigit(argv[2][i]) == 0) {
				printf("error: timeout must be an integer\n");
				exit(-1);
			}
		}
		wait = atoi(argv[2]);
	}
	
	else {
		printf("usage: ttt [-t timeout] / [-q]\n");
		exit(-1);
	}
	
	// query mode
	if (query) {
		
		memset( &hints, 0, sizeof(hints));
  		hints.ai_family = AF_INET; hints.ai_socktype = SOCK_DGRAM;
		hints.ai_flags = AI_NUMERICSERV; hints.ai_protocol = 0;
  		hints.ai_canonname = NULL; hints.ai_addr = NULL;
  		hints.ai_next = NULL;
			
		time = 0;
		wait = 60;
		notready = 1;
		while (time < wait && notready) {
				if ((file = fopen("temp.txt", "r")) == NULL) {
					sleep(1);
					time++;
					if (time == wait) {
						printf("timeout error: file does not exit\n");
						exit(0);
					}
				}
				else {
					notready = 0;
				}
		}
		file = fopen("temp.txt", "r");
		fgets(hostname, sizeof(hostname), file);
		if( (ptr = strchr(hostname, '\n')) != NULL) *ptr = '\0';
		fgets(buff, sizeof(buff), file);
		fgets(port, sizeof(port), file);
		if( (ptr = strchr(port, '\n')) != NULL) *ptr = '\0';
		fclose(file);
		
  		ecode = getaddrinfo(hostname, port, &hints, &addrlist);
  		if (ecode != 0) {
    		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ecode));
    		exit(-1);
  		}

  		server = (struct sockaddr_in *) addrlist->ai_addr;

  		if ( (sock = socket( addrlist->ai_family, addrlist->ai_socktype, 0 )) < 0 ) {
    		perror("ttt:socket");
    		exit(-1);
  		}
  		
  		strcpy(dgmsg, "q");
  		
  		checkDG = sendto(sock, &dgmsg, sizeof(dgmsg), 0, (struct sockaddr *) server, 
  		  sizeof(struct sockaddr_in));
  		  
  		if (checkDG < 0) {
  			perror("ttt:sendto");
  			exit(-1);
  		}
  		
		length_dg = sizeof(server);
	    dg_sender = recvfrom(sock, &dgmsg, sizeof(dgmsg), 0, (struct sockaddr *)&server,
	      &length_dg);
    	if (dg_sender < 0)
    		perror("TTT:recvfrom");
    	printf("%s\n", dgmsg);
  		
	}
	
	// game mode
	else {
	
		memset( &hints, 0, sizeof(hints));
  		hints.ai_family = AF_INET; hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_NUMERICSERV; hints.ai_protocol = 0;
  		hints.ai_canonname = NULL; hints.ai_addr = NULL;
  		hints.ai_next = NULL;
		
		time = 0;
		wait = 60;
		notready = 1;
		while (time < wait && notready) {
				if ((file = fopen("temp.txt", "r")) == NULL) {
					sleep(1);
					time++;
					if (time == wait) {
						printf("timeout error: file does not exit\n");
						exit(0);
					}
				}
				else {
					notready = 0;
				}
		}
		file = fopen("temp.txt", "r");
		fgets(hostname, sizeof(hostname), file);
		if( (ptr = strchr(hostname, '\n')) != NULL) *ptr = '\0';
		fgets(port, sizeof(port), file);
		if( (ptr = strchr(port, '\n')) != NULL) *ptr = '\0';
		fclose(file);
		
  		ecode = getaddrinfo(hostname, port, &hints, &addrlist);
  		if (ecode != 0) {
    		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ecode));
    		exit(-1);
  		}

  		server = (struct sockaddr_in *) addrlist->ai_addr;

  		if ( (sock = socket( addrlist->ai_family, addrlist->ai_socktype, 0 )) < 0 ) {
    		perror("ttt:socket");
    		exit(-1);
  		}
		
  		if ( connect(sock, (struct sockaddr *)server, sizeof(struct sockaddr_in)) < 0) {
    		perror("ttt:connect");
    		exit(-1);
  		}
		
		usrt.tv_sec = atoi(argv[2]);
		usrt.tv_usec = 0;
		
		FD_ZERO(&womp);
		
		while(1) {
		
			// get message
			count = 0;
			recv[0] = '\0';
			FD_SET(sock, &womp);
			if (timeout && notready) hits = select(sock + 1, &womp, NULL, NULL, &usrt);
			else hits = select(sock + 1, &womp, NULL, NULL, NULL);
			if (FD_ISSET(sock, &womp)) {
				while(count < MSGSIZE) {
					// TODO: take care of possible readsize issues
					if (read(sock, &recv, MSGSIZE) == MSGSIZE) {
						count += MSGSIZE;
						if (notready) notready = 0;
					}
				}
			}			
			
			if (recv[0] == 'j') {
				printf("there are currently people in game. please wait\n");
				notready = 1;
			}
			
			else if (recv[0] == 'k') {
				printf("there are no more available spots in line\n");
				exit(0);
			}
			
			else if (recv[0] == 'a') {
				printf("enter handle: ");
				msg[0] = '\0';
				scanf("%s", msg);
				for(i = strlen(msg); i < MSGSIZE; i++) {
					msg[i] = ' ';
				}
				wtsock(sock, msg);
				notready = 1;
			}
		
			else if(recv[0] == 'b') {
				printf("your opponent's handle is: ");
				for (j = 1; j < MSGSIZE; j++) {
					putchar(recv[j]);
				}
				putchar('\n');
			}

			else if (recv[0] == 'c') {
				printf("you are player X\n");
				player = 'X';
			}
			else if (recv[0] == 'd') {
				printf("you are player O\n");
				player = 'O';
			}

			else if (recv[0] == 'e') {
				invalid = 1;
				while (invalid) {
					printboard(recv);
					printf("enter move: ");
					scanf("%s", msg);
					if((index = isvalid(msg, recv))) {
						recv[index] = player;
						wtsock(sock, recv);
						invalid = 0;
					}
				}
			}
			// get game over (w/l/d/a)
			else if (recv[0] == 'f') {
				printboard(recv);
				printf("you win!\n");
				exit(0);
			}
			else if (recv[0] == 'g') {
				printboard(recv);
				printf("you lose!\n");
				exit(0);
			}
			else if (recv[0] == 'h') {
				printboard(recv);
				printf("a draw!\n");
				exit(0);
			}
			else if (recv[0] == 'i') {
				printf("your opponent surrendered!\n");
				exit(0);
			}
			else if (recv[0] != '\0') {
				printf("%c\n", recv[0]);
				printf("invalid state received from server\n");
				exit(-1);
			}
			else {
				printf("timeout\n");
				exit(0);
			}
		}
	}
	
	return 0;

}