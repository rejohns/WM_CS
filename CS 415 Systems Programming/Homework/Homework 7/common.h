#ifndef COMMON_H

#define COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

/************************************************************
 
 common.h	- Phil Kearns - sometime in the nether era
 
 All "messages" sent between server and clients in the
 distributed tic-tac-toe system are in the format defined
 by struct tttmsg. The structure of the message is determined
 by the type field:

 type=A   WHO              no other fields used
 type=B   HANDLE           data is the string handle for ttt
                            sending this message
 type=C   MATCH            data is string handle of opponent;
                            board[0] is 'X' or 'O' to denote
                            character this ttt is using
 type=D   WHATMOVE         board[] contains X/O/space chars
                            to denote current state
 type=E   MOVE             res (ascii) indicates square into
                            which client is moving
 type=F   RESULT           board[] contains X/O/space chars
                            to denote current state;
                            res =   W -> you win
                                    L -> you lose
                                    D -> draw
																		R -> opponent resigned
 type=G		RESIGN					client is resigning from match
													  and forfeits

 modified by Robert Johns - April 25, 2014
 
 adds message type G for resignation, result type R for 
 telling a client that the opponent resigned.

************************************************************/

#define WHO 'A'
#define HANDLE 'B'
#define MATCH 'C'
#define WHATMOVE 'D'
#define MOVE 'E'
#define RESULT 'F'
#define RESIGN 'G'

struct tttmsg{
  char type;     /* Message type */
  char board[9]; /* X/O */
  char data[32]; /* null-terminated string */
  char res;      /* integer data */
};

#define SFILE "./serverloc"

void putmsg();
void getmsg();
void protocol_error();
void dumpmsg();

#endif
