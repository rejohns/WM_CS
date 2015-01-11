// Using getaddrinfo() to get IPv4 address for
// hostname (argv[1]) and service (argv[2]).
//
// The print_XXX() functions are from Stevens
// and Rago.
//
// P. Kearns, February 2009


#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void
print_family(struct addrinfo *aip)
{
    printf(" family ");
    switch (aip->ai_family) {
    case AF_INET:
        printf("inet");
        break;
    case AF_INET6:
        printf("inet6");
        break;
    case AF_UNIX:
        printf("unix");
        break;
    case AF_UNSPEC:
        printf("unspecified");
        break;
    default:
        printf("unknown");
    }

}
void
print_type(struct addrinfo *aip)
{
    printf(" type ");
    switch (aip->ai_socktype) {
    case SOCK_STREAM:
        printf("stream");
        break;
    case SOCK_DGRAM:
        printf("datagram");
        break;
    case SOCK_SEQPACKET:
        printf("seqpacket");
        break;
    case SOCK_RAW:
        printf("raw");
        break;
    default:
        printf("unknown (%d)", aip->ai_socktype);
    }
}

void
print_protocol(struct addrinfo *aip)
{
    printf(" protocol ");
    switch (aip->ai_protocol) {
    case 0:
        printf("default");
        break;
    case IPPROTO_TCP:
        printf("TCP");
        break;
    case IPPROTO_UDP:
        printf("UDP");
        break;
    case IPPROTO_RAW:
        printf("raw");
        break;
    default:
        printf("unknown (%d)", aip->ai_protocol);
    }
}

void
print_flags(struct addrinfo *aip)
{
    printf("flags");
    if (aip->ai_flags == 0) {
        printf(" 0");

    } else {
        if (aip->ai_flags & AI_PASSIVE)
            printf(" passive");
        if (aip->ai_flags & AI_CANONNAME)
            printf(" canon");
        if (aip->ai_flags & AI_NUMERICHOST)
            printf(" numhost");
#if defined(AI_NUMERICSERV)
        if (aip->ai_flags & AI_NUMERICSERV)
            printf(" numserv");
#endif
#if defined(AI_V4MAPPED)
        if (aip->ai_flags & AI_V4MAPPED)
            printf(" v4mapped");
#endif
#if defined(AI_ALL)
        if (aip->ai_flags & AI_ALL)
            printf(" all");
#endif
    }
}

int main(int argc,char **argv)
{
  struct addrinfo *addrlist, *aptr, hints;
  struct sockaddr_in *saddrptr;
  const char * p; char addrbuffer[INET_ADDRSTRLEN]; int ecode;

  if(argc != 3) {
    fprintf(stderr, "usage: getaddrs node service\n");
    exit(1);
  }

// Want IPv4 lookup only.

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET; hints.ai_socktype = 0;
  hints.ai_flags = AI_CANONNAME; hints.ai_protocol = 0;
  hints.ai_canonname = NULL; hints.ai_addr = NULL;
  hints.ai_next = NULL;

// Do the lookup.

  ecode = getaddrinfo(argv[1], argv[2], &hints, &addrlist);
  if (ecode != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ecode));
    exit(1);
  }

// Scan the list of results.

  for (aptr = addrlist; aptr != NULL; aptr = aptr->ai_next) {
    print_flags(aptr); print_family(aptr);
    print_type(aptr); print_protocol(aptr);
    
    if (aptr->ai_family == AF_INET){
      printf("\tName: %s\n", aptr->ai_canonname?aptr->ai_canonname:"none");

      // Get at filled-in sockaddr_in hanging off the addrinfo.

      saddrptr = (struct sockaddr_in *) aptr->ai_addr;
      p = inet_ntop(AF_INET, &saddrptr->sin_addr.s_addr, addrbuffer, INET_ADDRSTRLEN);

      if (!p) {
        perror("inet_ntop");
        exit(1);
      }

      printf("IP Address: %s\n", p?p:"unknown");
      printf("Port: %d\n", ntohs(saddrptr->sin_port));
    }
  }

  // Give back result structs (not really impt here).

  freeaddrinfo(addrlist);
  exit(0);   
}
