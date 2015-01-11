/************************************************************************\
* 			  INET_RSTREAM.C					       			         *
* Test of TCP/IP. Set up a socket for establishing a connection at       *
* some free port on the local host. After accepting a connection, speak  *
* briefly to peer, and then read char at a time from the stream. Write   *
* to stdout. At EOF, shut down.                                          *
*								 								         *
* Phil Kearns													         *
* April 11, 1987												         *
* 																         *
* Modified February 2009 to use getaddrinfo()                            *
\************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

void printsin(struct sockaddr_in*, char*, char*);

int main()
{
  int listener;  /* fd for socket on which we get connection requests */
  int conn;      /* fd for socket thru which we pass data */
  struct sockaddr_in *localaddr, peer;
  int ecode;
  socklen_t length;
  char ch;
  struct addrinfo hints, *addrlist;


/* 
   Want to specify local server address of:
      addressing family: AF_INET
      ip address:        any interface on this system 
      port:              0 => system will pick free port
*/

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET; hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_NUMERICSERV | AI_PASSIVE; hints.ai_protocol = 0;
  hints.ai_canonname = NULL; hints.ai_addr = NULL;
  hints.ai_next = NULL;

  ecode = getaddrinfo(NULL, "0", &hints, &addrlist);
  if (ecode != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ecode));
    exit(1);
  }

  localaddr = (struct sockaddr_in *) addrlist->ai_addr;
 
/*
   Create socket on which we will accept connections. This is NOT the
   same as the socket on which we pass data.
*/
  if ( (listener = socket( addrlist->ai_family, addrlist->ai_socktype, 0 )) < 0 ) {
    perror("inet_rstream:socket");
    exit(1);
  }


  if (bind(listener, (struct sockaddr *)localaddr, sizeof(struct sockaddr_in)) < 0) {
    perror("inet_rstream:bind");
    exit(1);
  }

/*
   Print out the port number assigned to this process by bind().
*/
  length = sizeof(struct sockaddr_in);
  if (getsockname(listener, (struct sockaddr *)localaddr, &length) < 0) {
    perror("inet_rstream:getsockname");
    exit(1);
  }
  printf("RSTREAM:: assigned port number %d\n", ntohs(localaddr->sin_port));

/*
   Now accept a single connection. Upon connection, data will be
   passed through the socket on descriptor conn.
*/
  listen(listener,1);
  length = sizeof(peer);
  if ((conn=accept(listener, (struct sockaddr *)&peer, &length)) < 0) {
    perror("inet_rstream:accept");
    exit(1);
  }
  printsin(&peer,"RSTREAM::", "accepted connection from"); 

  printf("\n\nRSTREAM:: data from stream:\n");
  while ( read(conn, &ch, 1) == 1)
    putchar(ch);
  putchar('\n');
  exit(0);
}

void printsin(struct sockaddr_in *sin, char *m1, char *m2 )
{
  char fromip[INET_ADDRSTRLEN];

  printf ("%s %s:\n", m1, m2);
  printf ("  family %d, addr %s, port %d\n", sin -> sin_family,
	    inet_ntop(AF_INET, &(sin->sin_addr.s_addr), fromip, sizeof(fromip)),
            ntohs((unsigned short)(sin -> sin_port)));
}