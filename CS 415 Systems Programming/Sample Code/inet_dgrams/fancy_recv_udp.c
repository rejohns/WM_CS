/*********************************************************************\
* 		       FANCY_RECV_UDP.C				  					      *
* Test of UDP/IP. Receive datagrams on internet socket bound to port  *
* 0x3333 on the local host.										      *
* 																      *
* To use:														      *
* 	1) On the appropriate host, invoke this code by				      *
* 	   "fancy_recv_udp&".					   					      *
* 	2) Invoke send_udp as many times as desired on the	  		      *
* 	   remote host.												      *
* 																      *
* Phil Kearns				 									      *
* April 11, 1987												      *
* 																      *
* Modified: April 10, 1992                                            *
*      responds to input (CR) on stdin by cleanly shutting down       *
*      also shuts down if no datagrams for a minute                   *
\*********************************************************************/

#include	<signal.h>
#include	<errno.h>
#include	<string.h>
#include	<stdio.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<netdb.h>
#include    <arpa/inet.h>
#include    <stdlib.h>

void printsin(struct sockaddr_in*, char*, char*);

int main()
{
  int	socket_fd, cc, hits, ecode;
  socklen_t fsize;
  fd_set mask;
  struct timeval timeout;
  struct sockaddr_in	*s_in, from;
  struct addrinfo hints, *addrlist;
    
  struct {
    char    head;
    u_long  body;
    char    tail;
  } msg;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET; hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_NUMERICSERV | AI_PASSIVE; hints.ai_protocol = 0;
  hints.ai_canonname = NULL; hints.ai_addr = NULL;
  hints.ai_next = NULL;

  ecode = getaddrinfo(NULL, "13107", &hints, &addrlist);
  if (ecode != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ecode));
    exit(1);
  }

  s_in = (struct sockaddr_in *) addrlist->ai_addr;

  printsin(s_in, "FANCY_RECV_UDP", "Local socket is:"); fflush(stdout);


  socket_fd = socket (addrlist->ai_family, addrlist->ai_socktype, 0);
  if (socket_fd < 0) {
    perror ("fancy_recv_udp:socket");
    exit (1);
  }

/*
   bind port 0x3333 on the current host to the socket accessed through
   socket_fd. If port in use, die.
*/
  if (bind(socket_fd, (struct sockaddr *)s_in, sizeof(struct sockaddr_in)) < 0) {
    perror("fancy_recv_udp:bind");
    exit(1);
  }

  for(;;) {
    fsize = sizeof(from);
/* Here's the new stuff. Hang a select on the file descriptors 0 (stdin)
   and socket_fd looking to see if either descriptor is able to be read.
   If it's stdin, shut down. If it's socket_fd, proceed as normal. If
   Nothing happens for a minute, shut down also.
*/
    FD_ZERO(&mask);
    FD_SET(0,&mask);
    FD_SET(socket_fd,&mask);
    timeout.tv_sec = 60;
    timeout.tv_usec = 0;
    if ((hits = select(socket_fd+1, &mask, (fd_set *)0, (fd_set *)0,
                           &timeout)) < 0) {
      perror("fancy_recv_udp:select");
      exit(1);
    }
    if ( (hits==0) || ((hits>0) && (FD_ISSET(0,&mask))) ) {
      printf("Shutting down\n");
      exit(0);
    }
    cc = recvfrom(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&from,&fsize);
    if (cc < 0) perror("recv_udp:recvfrom");
    printsin( &from, "recv_udp: ", "Packet from:");
    printf("Got data ::%c%u%c\n",msg.head,ntohl(msg.body),msg.tail);
    fflush(stdout);
  }
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