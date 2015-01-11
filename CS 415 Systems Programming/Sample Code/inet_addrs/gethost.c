// Using getnameinfo() to get host name for a
// given IP address (argv[1]).
//
// P. Kearns, February 2009

#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
  
  int ecode;
  struct in_addr fillin;
  struct sockaddr_in addr;
  char host[NI_MAXHOST];

  if (argc != 2) {
    fprintf(stderr,"usage: gethost address\n");
    exit(1);
  }	

// Dotted decimal to binary IP address.

  ecode = inet_pton(AF_INET, argv[1], &fillin);
  if (ecode == 0) {
        fprintf(stderr,"inet_pton: invalid address\n");
        exit(1);
  }
  if (ecode < 0) {
	perror("inet_pton"); exit(1);
        exit(1);
  }

// Fill in blanks of a sockaddr_in after zeroing it out.

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(0);
  memcpy(&(addr.sin_addr), &fillin, sizeof(addr.sin_addr));

// Do the lookup.

  ecode = getnameinfo((struct sockaddr *) &addr, sizeof(addr),
                          host, NI_MAXHOST, NULL, 0, NI_NAMEREQD);
  if (ecode) {
    fprintf(stderr, "getnameinfo: %s\n", gai_strerror(ecode));
    exit(1);
  }

  printf("Hostname: %s\n", host); 
  exit(0);
}