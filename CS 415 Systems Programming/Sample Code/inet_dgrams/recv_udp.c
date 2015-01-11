/*********************************************************************\
* 			  RECV_UDP.C										      *
* Test of UDP/IP. Receive datagrams on internet socket bound to port  *
* 0x3333 on the local host.										      *
* 																      *
* To use:														      *
* 	1) On the appropriate host, invoke this code by				      *
* 	   "recv_udp&".												      *
* 	2) Invoke send_udp as many times as desired on the			      *
* 	   remote host.												      *
* 	3) When done, MAKE SURE TO KILL THIS BACKGROUND PROCESS!	      *
*								 								      *
* Phil Kearns													      *
* April 11, 1987												      *
* 																      *
* Modified February 2009: use getaddrinfo()                           *
\*********************************************************************/

#include    <stdlib.h>
#include	<signal.h>
#include	<errno.h>
#include	<string.h>
#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<netdb.h>
#include    <arpa/inet.h>

void printsin(struct sockaddr_in*, char*, char*);

int main()
{
    int	socket_fd, cc, ecode;
    socklen_t fsize;
    struct sockaddr_in	*s_in, from;
    struct addrinfo hints, *addrlist; 
    void printsin();

    struct {
	char	head;
	u_long	body;
	char	tail;
    } msg;

/*
   In order to attach a name to the socket created above, first fill
   in the appropriate blanks in an inet socket address data structure
   called "s_in". We blindly pick port number 0x3333. The second step
   is to BIND the address to the socket. If port 0x3333 is in use, the
   bind system call will fail detectably.
*/

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET; hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_NUMERICSERV | AI_PASSIVE; hints.ai_protocol = 0;
  hints.ai_canonname = NULL; hints.ai_addr = NULL;
  hints.ai_next = NULL;

/*
 getaddrinfo() should return a single result, denoting
 a SOCK_DGRAM socket on any interface of this system at
 port 0x3333.
*/

  ecode = getaddrinfo(NULL, "13107", &hints, &addrlist);
  if (ecode != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ecode));
    exit(1);
  }

  s_in = (struct sockaddr_in *) addrlist->ai_addr;

  printsin(s_in, "RECV_UDP", "Local socket is:"); fflush(stdout);

/*
   Create the socket to be used for datagram reception. Initially,
   it has no name in the internet (or any other) domain.
*/
  socket_fd = socket (addrlist->ai_family, addrlist->ai_socktype, 0);
  if (socket_fd < 0) {
    perror ("recv_udp:socket");
    exit (1);
  }

/*
   bind port 0x3333 on the current host to the socket accessed through
   socket_fd. If port in use, bind() will fail and we die.
*/

  if (bind(socket_fd, (struct sockaddr *)s_in, sizeof(struct sockaddr_in)) < 0) {
    perror("recv_udp:bind");
    exit(1);
  }

  for(;;) {
    fsize = sizeof(from);
    cc = recvfrom(socket_fd, &msg, sizeof(msg), 0, (struct sockaddr *)&from, &fsize);
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
