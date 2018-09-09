/* TTT.c	Robert Johns	March 21, 2014
 * This program acts as a server between tic-tac toe clients, which can also accept and
 * process data requests without delays to either.  It can handle sudden death of a 
 * client, and up to four clients waiting at once.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
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
    		perror("TTT:write");
    		exit(-1);
    	}
    	else left -= num;
    	put += num;
  	}
}

char winningboard(char board[]) {
	
	int count;
	if (
	  (board[1] == board[4] && board[4] == board[7]) ||
	  (board[1] == board[2] && board[2] == board[3]) ||
	  (board[1] == board[5] && board[5] == board[9])
	     ) {
	 	  return board[1];
	}
	else if (
	  (board[2] == board[5] && board[5] == board[8]) ||
	  (board[3] == board[6] && board[6] == board[9]) ||
  	  (board[4] == board[5] && board[5] == board[6])
  	        ) {
		return board[6];
	}
	else if (
	  (board[3] == board[5] && board[5] == board[7]) ||
 	  (board[7] == board[8] && board[8] == board[9])
 	        ) {
		return board[7];
	}
	for (count = 1; count <= 9; count++) {
		if (atoi(&board[count]) == count) {
			return 'c';
		}
	}
	return 'd';
}

int findmax (int p1, int p2, int w1, int w2, int w3, int w4, int s, int ls, int ld) {
	int max = p1;
	if (p2 > max) max = p2;
	if (w1 > max) max = w1;
	if (w2 > max) max = w2;
	if (w3 > max) max = w3;
	if (w4 > max) max = w4;
	if (s > max) max = s;
	if (ls > max) max = ls;
	if (ld > max) max = ld;
	return max;
}

int main(void) {
	
	char hostname[1024]; 							   // name of host
	FILE *file;										   // file to write to
	char msg[MSGSIZE]; 								   // message to send
	char dgmsg[1024]; 								   // datagram message
	int listen_stream, listen_dg; 					   // listeners
	int player1 = -1, player2 = -1;					   // players
	int wait1, wait2, wait3, wait4; 				   // waiters
	int schmuck; 									   // socket for rejections
	int dg_sender; 									   // variable for sending dg error
	struct sockaddr_in *localaddr_stream, peer_stream; // addr stuff for stream
	struct sockaddr_in *localaddr_dg, peer_dg; 		   // addr stuff for datagram
	int ecode_stream, ecode_dg; 					   // variable for getaddrinfo error
	int count = 0; 									   // variable for reading from stream
	socklen_t length_stream, length_dg; 			   // socket lengths
	struct addrinfo hints_stream, *addrlist_stream;    // addrinfo for streams
	struct addrinfo hints_dg, *addrlist_dg; 		   // addrinfo for datagrams
	fd_set reads, writes; 					   // file descriptor sets
	int hits;	 									   // select return value
	int max;										   // max socket fd
	char handle1[MSGSIZE], handle2[MSGSIZE]; 		   // arrays for player handles
	int line; 										   // number of people in line
	int state; 										   // current state of game
	char board[] = {"e123456789"};					   // board variable
	char boardS[] = {"e123456789"};					   // starting state for board
	char handlebuff1[MSGSIZE];
	char handlebuff2[MSGSIZE];
	int i;
	int bytes;
	
	if (argc > 1) {
		printf("usage: TTT\n");
		exit(-1);
	}
	
	 // host name
	unlink("temp.txt");
	gethostname(hostname, 1024);
	file = fopen("temp.txt", "w");
	fprintf(file, "%s\n", hostname);
	fclose(file);
	
	// set up vc port
	memset(&hints_stream, 0, sizeof(hints_stream));
	hints_stream.ai_family = AF_INET; hints_stream.ai_socktype = SOCK_STREAM;
	hints_stream.ai_flags = AI_NUMERICSERV | AI_PASSIVE; hints_stream.ai_protocol = 0;
	hints_stream.ai_canonname = NULL; hints_stream.ai_addr = NULL;
	hints_stream.ai_next = NULL;

	ecode_stream = getaddrinfo(NULL, "0", &hints_stream, &addrlist_stream);
	if (ecode_stream != 0) {
    	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ecode_stream));
    	exit(-1);
  	}

	localaddr_stream = (struct sockaddr_in *) addrlist_stream->ai_addr;
 
  	if ((listen_stream = socket( addrlist_stream->ai_family, addrlist_stream->ai_socktype, 0 )) < 0 ) {
    	perror("TTT:socket");
    	exit(-1);
  	}

  	if (bind(listen_stream, (struct sockaddr *)localaddr_stream, sizeof(struct sockaddr_in)) < 0) {
    	perror("TTT:bind");
    	exit(-1);
  	}

  	length_stream = sizeof(struct sockaddr_in);
  	if (getsockname(listen_stream, (struct sockaddr *)localaddr_stream, &length_stream) < 0) {
   		perror("TTT:getsockname");
    	exit(-1);
  	}
  	
  	file = fopen("temp.txt", "a");
  	fprintf(file, "%d\n", ntohs(localaddr_stream->sin_port));
	fclose(file);
	// end setting up of vc port
	
	// set up datagram port
	memset(&hints_dg, 0, sizeof(hints_dg));
	hints_dg.ai_family = AF_INET; hints_dg.ai_socktype = SOCK_DGRAM;
	hints_dg.ai_flags = AI_NUMERICSERV | AI_PASSIVE; hints_dg.ai_protocol = 0;
	hints_dg.ai_canonname = NULL; hints_dg.ai_addr = NULL;
	hints_dg.ai_next = NULL;
  	
  	ecode_dg = getaddrinfo(NULL, "0", &hints_dg, &addrlist_dg);
  	if (ecode_dg != 0) {
  		fprintf(stderr, "getaddrinfoDG: %s\n", gai_strerror(ecode_dg));
  		exit(-1);
  	}
  	
  	localaddr_dg = (struct sockaddr_in *) addrlist_dg->ai_addr;
  	
  	listen_dg = socket(addrlist_dg->ai_family, addrlist_dg->ai_socktype, 0);
  	if(listen_dg < 0) {
  		perror("TTT:socketDG");
  		exit(-1);
  	}
  	
  	if(bind(listen_dg, (struct sockaddr *)localaddr_dg, sizeof(struct sockaddr_in)) < 0){
  		perror("TTT:bindDG");
  		exit(-1);
  	}
  	
  	length_dg = sizeof(struct sockaddr_in);
  	hits = getsockname(listen_dg, (struct sockaddr *)localaddr_dg, &length_dg);
  	if (hits < 0){
  		perror("TTT:getsocknameDG");
  		return -1;
  	}
  	
  	file = fopen("temp.txt", "a");
  	fprintf(file, "%d\n", ntohs(localaddr_dg->sin_port));
	fclose(file);
  	//end setting up of datagram port
	
	// new code
	
	listen(listen_stream,1);
	FD_ZERO(&reads);
	FD_ZERO(&writes);
	state = 0;
	line = 0;
	length_stream = sizeof(peer_stream);
	length_dg = sizeof(peer_dg);
	max = listen_dg;
	if (listen_stream > max) max = listen_stream;
	wait1 = -1; wait2 = -1; wait3 = -1; wait4 = -1; schmuck = -1;
	
	// run daemon
	while(1) {
		//printf("%d\n", state);
		count = 0;
		if (state == 17) {
			player1 = -1;
			player2 = -1;
			line -= 2;
			strcpy(board, boardS);
			if (line == 0) {
				state = 0;
			}
			if (line == 1) {
				player1 = wait1;
				wait1 = -1;
				FD_SET(player1, &writes);
				state = 1;
			}
			else if (line == 2) {
				player1 = wait1;
				player2 = wait2;
				wait1 = -1;
				wait2 = -1;
				FD_SET(player1, &writes);
				state = 1;
			}
			else if (line == 3) {
				player1 = wait1;
				player2 = wait2;
				wait1 = wait3;
				wait2 = -1;
				FD_SET(player1, &writes);
				state = 1;
			}
			else if (line == 4) {
				player1 = wait1;
				player2 = wait2;
				wait1 = wait3;
				wait2 = wait4;
				wait3 = -1;
				wait4 = -1;
				FD_SET(player1, &writes);
				state = 1;
			}
		}
		FD_SET(listen_dg, &reads);
		FD_SET(listen_stream, &reads);
		max = findmax(player1, player2, wait1, wait2, wait3, wait4, schmuck, listen_dg,
		  listen_stream);
		
		hits = select(max + 1, &reads, &writes, NULL, NULL);
		if(hits < 0) {
			perror("TTT:select");
			exit(-1);
		}
		// listen for dg connections
		if (FD_ISSET(listen_dg, &reads)) {
    		dg_sender = recvfrom(listen_dg, &msg, sizeof(msg), 0, 
    		  (struct sockaddr *)&peer_dg, &length_dg);
    		if (dg_sender < 0) {
    			perror("TTT:recvfrom");
				exit(-1);
			}
			FD_SET(listen_dg, &writes);
		}
		else if (FD_ISSET(listen_dg, &writes)) {
			if(line == 0) {
				strcpy(dgmsg, "there are currently no players");
			}
			else if (line == 1) {
				for (i = 1; handle1[i] != ' '; i++) {
					handlebuff1[i - 1] = handle1[i];
				}
				snprintf(dgmsg, sizeof(dgmsg), "one player waiting: <%s>", handlebuff1);
			}
			else if (line == 2) {
				for (i = 1; handle1[i] != ' '; i++) {
					handlebuff1[i - 1] = handle1[i];
				}
				for (i = 1; handle2[i] != ' '; i++) {
					handlebuff2[i - 1] = handle2[i];
				}
				snprintf(dgmsg,sizeof(dgmsg),
				 "two players: <%s> and <%s>", handlebuff1, handlebuff2);
			}
			else {
				for (i = 1; handle1[i] != ' '; i++) {
					handlebuff1[i - 1] = handle1[i];
				}
				for (i = 1; handle2[i] != ' '; i++) {
					handlebuff2[i - 1] = handle2[i];
				}
				snprintf(dgmsg,sizeof(dgmsg), "%d players: <%s> and <%s> and %d waiting",
				 line, handlebuff1, handlebuff2, line - 2);
			}
	
			dg_sender = sendto(listen_dg, &dgmsg, sizeof(dgmsg), 0,
			  (struct sockaddr *)&peer_dg, sizeof(struct sockaddr_in));
			if(dg_sender < 0) {
				perror("TTT:sendto");
				exit(-1);
			}
			FD_CLR(listen_dg, &writes);
			FD_SET(listen_dg, &reads);
			if(state == 2 || state == 11) {
				FD_SET(player1, &reads);
			}
			else if(state == 5 || state == 13) {
				FD_SET(player2, &reads);
			}
		}
		// listen for stream connections
		else if (FD_ISSET(listen_stream, &reads)) {
			if (line < 0) {
				printf("error: line is negative\n");
				exit(-1);
			}
			// line empty
			if (line == 0) {		
				if ((player1 = accept(listen_stream, (struct sockaddr *)&peer_stream,
				  &length_stream)) < 0) {
     				perror("TTT:accept");
    				exit(-1);
  				}
				if (player1 > max) max = player1;
				FD_SET(player1, &writes);
				FD_CLR(player1, &reads);
  				state = 1;
				line++;
			}
			// one pending player
			else if (line == 1) {
				if ((player2 = accept(listen_stream, (struct sockaddr *)&peer_stream, 
				  &length_stream)) < 0){
					perror("TTT: accept");
					exit(-1);
				}
				if (player2 > max) max = player2;
				FD_SET(player2, &writes);
				FD_CLR(player2, &reads);
				state = 4;
				line++;
			}
			// can take waiters
			else if (line == 2) {
				if ((wait1 = accept(listen_stream, (struct sockaddr *)&peer_stream, 
				  &length_stream)) < 0){
					perror("TTT: accept");
					exit(-1);
				}
				if (wait1 > max) max = wait1;
				FD_SET(wait1, &writes);
				line++;
			}
			else if (line == 3) {
				if ((wait2 = accept(listen_stream, (struct sockaddr *)&peer_stream, 
				  &length_stream)) < 0){
					perror("TTT: accept");
					exit(-1);
				}
				if (wait2 > max) max = wait2;
				FD_SET(wait2, &writes);
				line++;
			}
			else if (line == 4) {
				if ((wait3 = accept(listen_stream, (struct sockaddr *)&peer_stream, 
				  &length_stream)) < 0){
					perror("TTT: accept");
					exit(-1);
				}
				if (wait3 > max) max = wait3;
				FD_SET(wait3, &writes);
				line++;
			}
			else if (line == 5) {
				if ((wait4 = accept(listen_stream, (struct sockaddr *)&peer_stream, 
				  &length_stream)) < 0){
					perror("TTT: accept");
					exit(-1);
				}
				if (wait4 > max) max = wait4;
				FD_SET(wait4, &writes);
				line++;
			}
			// can't take waiters
			else {
				if ((schmuck = accept(listen_stream, (struct sockaddr *)&peer_stream, 
				  &length_stream)) < 0){
					perror("TTT: accept");
					exit(-1);
				}
				if (schmuck > max) max = schmuck;
				FD_SET(schmuck, &writes);
				line++;
			}
		}
		// TODO: handle reads for guys in line who leave?
		// reads
		// 		player 1
		else if (FD_ISSET(player1, &reads)) {
			// reading handle from player 1
			// hang until player 2's handle arrives
			//  	2->3
			msg[0] = '\0';
			count = 0;
			bytes = 1;
			while (count < MSGSIZE && bytes > 0) {
				if ((bytes = read(player1, &msg, MSGSIZE)) <= MSGSIZE) {
					count += bytes;
				}
			}
			
			if(state == 2) {
				for (count = 30; count >= 0; count--) {
					msg[count + 1] = msg[count];
				}
				msg[32] = '\0';
				msg[0] = 'b';
				strcpy(handle1, msg);
				FD_CLR(player1, &reads);
				if(player2 > 0) {
					FD_SET(player2, &writes);
					state = 4;
				}
				else state = 3;
			}
			// reading move from player 1
			// writing move to player 2
			//  	11->12 or 11->15
			else if (state == 11) {
				if (bytes == 0 && count == 0) {
					board[0] = 'i';
					state = 16;
					FD_CLR(player1, &reads);
					FD_SET(player2, &writes);
				}
				else {
					strcpy(board, msg);
					FD_CLR(player1, &reads);
					if (winningboard(board) == 'X') {
						FD_SET(player1, &writes);
						board[0] = 'f';
						state = 15;
					}
					else if (winningboard(board) == 'O') {
						FD_SET(player1, &writes);
						board[0] = 'g';
						state = 15;
					}
					else if (winningboard(board) == 'd') {
						FD_SET(player1, &writes);
						board[0] = 'h';
						state = 15;
					}
					else if (winningboard(board) == 'c') {
						FD_CLR(player1, &reads);
						FD_SET(player2, &writes);
						state = 12;
					}
					else {
						FD_CLR(player1, &reads);
						FD_SET(player2, &writes);
						state = 16;
					}
				}
			}
			// error cases
			else if (state != 16) {
				printf("state: %d\n", state);
				printf("invalid state in player1 read\n");
				exit(-1);
			}
		}
		// 		player 2
		else if (player2 > 0 && FD_ISSET(player2, &reads)) {
			msg[0] = '\0';
			count = 0;
			bytes = 1;
			while (count < MSGSIZE && bytes > 0) {
				if ((bytes = read(player2, &msg, MSGSIZE)) <= MSGSIZE) {
					count += bytes;
				}
			}
			// reading handle from player 2
			// write player 2's handle to player 1
			//  	5->6
			if (state == 5) {
				for (count = 30; count >= 0; count--) {
					msg[count + 1] = msg[count];
				}
				msg[32] = '\0';
				msg[0] = 'b';
				strcpy(handle2, msg);
				FD_CLR(player2, &reads);
				FD_SET(player1, &writes);
				state = 6;				
			}
			// reading move from player 2
			// write move to player 1
			//  	13->14 or 13->15
			else if (state == 13) {
				if (bytes == 0 && count == 0) {
						board[0] = 'i';
						state = 18;
						FD_CLR(player2, &reads);
						FD_SET(player1, &writes);
				}
				else {
					strcpy(board, msg);
					FD_CLR(player2, &reads);
					if (winningboard(board) == 'X') {
						FD_SET(player1, &writes);
						board[0] = 'f';
						state = 15;
					}
					else if (winningboard(board) == 'O') {
						FD_SET(player1, &writes);
						board[0] = 'g';
						state = 15;
					}
					else if (winningboard(board) == 'd') {
						FD_SET(player1, &writes);
						board[0] = 'h';
						state = 15;
					}
					else if (winningboard(board) == 'c') {
						FD_SET(player1, &writes);
						state = 14;
					}
					else {
						FD_CLR(player2, &reads);
						FD_SET(player1, &writes);
						state = 18;
					}
				}
			}
			// error cases
			else if (state != 14) {
				printf("state: %d\n", state);
				printf("invalid state in player2 read\n");
				exit(-1);
			}
		}
		// writes
		//  	player 1
		else if (player1 > 0 && FD_ISSET(player1, &writes)) {
			// writing handle prompt to player 1
			// read handle from player 1
			//  	1->2
			if (state == 1) {
				wtsock(player1, "a");
				FD_CLR(player1, &writes);
				FD_SET(player1, &reads);
				state = 2;
			}
			// writing opponent's handle to player 1
			// write opponent's handle to player 2
			//  	6->7
			else if (state == 6) {
				wtsock(player1, handle2);
				FD_CLR(player1, &writes);
				FD_SET(player2, &writes);
				state = 7;
			}
			// writing x to player 1
			// write o to player 2
			//  	8->9
			else if (state == 8) {
				wtsock(player1, "c");
				FD_CLR(player1, &writes);
				FD_SET(player2, &writes);
				state = 9;
			}
			// writing move prompt to player 1
			// read move from player 1
			//  	14->11
			else if (state == 14) {
				wtsock(player1, board);
				FD_CLR(player1, &writes);
				FD_SET(player1, &reads);
				state = 11;
			}
			// writing game over to player 1
			// write game over to player 2
			//  	15->16
			else if (state == 15) {
				wtsock(player1, board);
				FD_CLR(player1, &writes);
				FD_SET(player2, &writes);
				// if player 2 aborted 
				state = 16;
			}
			else if (state == 18) {
				wtsock(player1, board);
				FD_CLR(player1, &writes);
				state = 17;
			}
			// error code
			else {
				printf("state: %d\n", state);
				printf("invalid state in player1 write\n");
				exit(-1);
			}
		}
		//		player 2
		else if (player2 > 0 && FD_ISSET(player2, &writes)) {
			// writing handle prompt to player 2
			// read handle from player 2
			//  	4->5
			if (state == 4) {
				wtsock(player2, "a");
				FD_CLR(player2, &writes);
				FD_SET(player2, &reads);
				state = 5;
			}
			// writing player 1's handle to player 2
			// writing x to player 1
			//  	7->8
			else if (state == 7) {
				wtsock(player2, handle1);
				FD_CLR(player2, &writes);
				FD_SET(player1, &writes);
				state = 8;
			}
			// writing o to player 2
			// write move prompt to player 1
			//  	9->14
			else if (state == 9) {
				wtsock(player2, "d");
				FD_CLR(player2, &writes);
				FD_SET(player1, &writes);
				state = 14;
			}
			// writing move prompt to player 2
			// read move from player 2
			//  	12->13
			else if (state == 12) {
				wtsock(player2, board);
				FD_CLR(player2, &writes);
				FD_SET(player2, &reads);
				state = 13;
			}
			// writing game over to player 2
			// end game
			//  	16->17
			else if (state == 16) {
				if (board[0] == 'f') {
					board[0] = 'g';
				}
				else if (board[0] == 'g') {
					board[0] = 'f';
				}
				else if (board[0] == 'h') {
					board[0] = 'h';
				}
				else {
					board[0] = 'i';
				}
				wtsock(player2, board);
				FD_CLR(player2, &writes);
				// if player 1 aborted
				state = 17;
			}
			// error cases
			else {
				printf("state: %d\n", state);
				printf("invalid state in player2 write\n");
				exit(-1);
			}
		}
		else if (wait1 > 0 && FD_ISSET(wait1, &writes)) {
			wtsock(wait1, "j");
			FD_CLR(wait1, &writes);
			if(state == 2 || state == 11) {
				FD_SET(player1, &reads);
			}
			else if(state == 5 || state == 13) {
				FD_SET(player2, &reads);
			}
		}
		else if (wait2 > 0 && FD_ISSET(wait2, &writes)) {
			wtsock(wait2, "j");
			FD_CLR(wait2, &writes);
			if(state == 2 || state == 11) {
				FD_SET(player1, &reads);
			}
			else if(state == 5 || state == 13) {
				FD_SET(player2, &reads);
			}
		}
		else if (wait3 > 0 && FD_ISSET(wait3, &writes)) {
			wtsock(wait3, "j");
			FD_CLR(wait3, &writes);
			if(state == 2 || state == 11) {
				FD_SET(player1, &reads);
			}
			else if(state == 5 || state == 13) {
				FD_SET(player2, &reads);
			}
		}
		else if (wait4 > 0 && FD_ISSET(wait4, &writes)) {
			wtsock(wait4, "j");
			FD_CLR(wait4, &writes);
			if(state == 2 || state == 11) {
				FD_SET(player1, &reads);
			}
			else if(state == 5 || state == 13) {
				FD_SET(player2, &reads);
			}
		}
		else if (schmuck > 0 && FD_ISSET(schmuck, &writes)) {
			wtsock(schmuck, "k");
			FD_CLR(schmuck, &writes);
			if(state == 2 || state == 11) {
				FD_SET(player1, &reads);
			}
			else if(state == 5 || state == 13) {
				FD_SET(player2, &reads);
			}
			schmuck = -1;
		}
	}
	
  	unlink("./temp.txt");
	
	return 0;

}