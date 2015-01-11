/***********************************************************************\
* 			   SEND_UDP.C										        *
* Test of UDP/IP. Send a single dippy datagram to a receiver process    *
* assumed to exist on port 0x3333 on the internet host specified as     *
* the single argument to this program.							        *
* 								     								    *
* To use:														        *
* 	1) Make sure that recv_udp is running (probably in the	 	        *
* 	   background on the target host.			        				*
* 	2) Issue the command "send_udp xx", where xx is a host name. 	    *
* 								 								        *
* Phil Kearns													        *
* April 11, 1987												        *
* 																        *
* Modified February 2009: use getaddrinfo()                             *
\***********************************************************************/

#include    <stdlib.h>
#include    <unistd.h>
#include	<signal.h>
#include	<errno.h>
#include	<string.h>
#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<netdb.h>

int main(int argc,char **argv)
{
  int	socket_fd, cc, ecode;
  struct sockaddr_in *dest;
  struct addrinfo hints, *addrlist;  
  struct {
    char   head;
    u_long body;
    char   tail;
  } msgbuf;


/*
   Use getaddrinfo to create a SOCK_DGRAM sockarddr_in set
   up for the host specified as argv[1] and port 0x3333.
*/
    
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET; hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_NUMERICSERV; hints.ai_protocol = 0;
  hints.ai_canonname = NULL; hints.ai_addr = NULL;
  hints.ai_next = NULL;

  ecode = getaddrinfo(argv[1], "13107", &hints, &addrlist);
  if (ecode != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ecode));
    exit(1);
  }

  dest = (struct sockaddr_in *) addrlist->ai_addr; // Will use in sendto().

/*
   Set up a datagram (UDP/IP) socket in the Internet domain.
   We will use it as the handle thru which we will send a
   single datagram. Note that, since this no message is ever
   addressed TO this socket, we do not bind an internet address
   to it. It will be assigned a (temporary) address when we send
   a message thru it.
*/

  socket_fd = socket (addrlist->ai_family, addrlist->ai_socktype, 0);
  if (socket_fd == -1) {
    perror ("send_udp:socket");
    exit (1);
  }

  msgbuf.head = '<';
  msgbuf.body = htonl(getpid());
  msgbuf.tail = '>';

  cc = sendto(socket_fd,&msgbuf,sizeof(msgbuf),0,(struct sockaddr *) dest,
                  sizeof(struct sockaddr_in));
  if (cc < 0) {
    perror("send_udp:sendto");
    exit(1);
  }

  exit(0);
}