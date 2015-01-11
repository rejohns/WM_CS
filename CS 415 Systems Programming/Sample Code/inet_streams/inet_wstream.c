/************************************************************************\
* 			  INET_WSTREAM.C					       			         *
* Test of TCP/IP. Invoke as                                              *
*             inet_wstream hostname portnumber                           *
* in order to send a fragment of an obnoxious Dijkstra quote to a        *
* process assumed to be reading from a stream at the specified           *
* address.                                                               *
* 																         *
* Phil Kearns													         *
* April 11, 1987												         *
*								 								         *
* Modified February 2009 to use getaddrinfo()                            * 
\************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

char msg[] = { "false pearls before real swine" };

int main(argc, argv)
int argc;
char **argv;
{
  int sock, left, num, put, ecode;
  struct sockaddr_in *server;
  struct addrinfo hints, *addrlist;

  if (argc != 3) {
    fprintf(stderr,"inet_wstream:usage is inet_wstream host port\n");
    exit(1);
  }



/*
   Want a sockaddr_in containing the ip address for the system
   specified in argv[1] and the port specified in argv[2].
*/

  memset( &hints, 0, sizeof(hints));
  hints.ai_family = AF_INET; hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_NUMERICSERV; hints.ai_protocol = 0;
  hints.ai_canonname = NULL; hints.ai_addr = NULL;
  hints.ai_next = NULL;

  ecode = getaddrinfo(argv[1], argv[2], &hints, &addrlist);
  if (ecode != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ecode));
    exit(1);
  }

  server = (struct sockaddr_in *) addrlist->ai_addr;

/*
   Create the socket.
*/
  if ( (sock = socket( addrlist->ai_family, addrlist->ai_socktype, 0 )) < 0 ) {
    perror("inet_wstream:socket");
    exit(1);
  }

/*
   Connect to data socket on the peer at the specified Internet
   address.
*/
  if ( connect(sock, (struct sockaddr *)server, sizeof(struct sockaddr_in)) < 0) {
    perror("inet_wstream:connect");
    exit(1);
  }

/*
   Write the quote and terminate. This will close the connection on
   this end, so eventually an "EOF" will be detected remotely.
*/
  left = sizeof(msg); put=0;
  while (left > 0){
    if((num = write(sock, msg+put, left)) < 0) {
      perror("inet_wstream:write");
      exit(1);
    }
    else left -= num;
    put += num;
  }
  exit(0);
}
