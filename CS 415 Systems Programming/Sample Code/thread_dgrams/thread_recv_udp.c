/*********************************************************************\
* 			  THREAD_RECV_UDP.C						     		      *
* Test of UDP/IP. Receive datagrams on internet socket bound to port  *
* 0x3333 on the local host.										      *
*								 								      *
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
*                                                                     *
* multithreaded March 2002                                            *
*                                                                     *
\*********************************************************************/

#include    <stdlib.h>
#include	<signal.h>
#include	<errno.h>
#include	<strings.h>
#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<netdb.h>
#include    <arpa/inet.h>
#include    <pthread.h>

extern	int errno;

struct msgtype {
  char	 head;
  u_long body;
  char   tail;
};

struct msgtype buffer;
int full=0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;   // A fast mutex.
pthread_cond_t gotsome = PTHREAD_COND_INITIALIZER;  // A default condition.
pthread_cond_t isempty = PTHREAD_COND_INITIALIZER;  // A default condition.

// Thread to wait for incoming datagrams, read them, and deposit
// them in a buffer for another thread to output.
void *
receiver()
{
  int socket_fd, cc;
  socklen_t fsize;
  struct sockaddr_in s_in, from;

  struct msgtype input;

  socket_fd = socket (AF_INET, SOCK_DGRAM, 0);
  if (socket_fd < 0) {
    perror ("recv_udp:socket");
    exit (1);
  }
     
  bzero((char *) &s_in, sizeof(s_in));

  s_in.sin_family = (short)AF_INET;
  s_in.sin_addr.s_addr = htonl(INADDR_ANY);
  s_in.sin_port = htons((u_short)0x3333); 
    
  if (bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in)) < 0) {
    perror("recv_udp:bind");
    exit(1);
  }

  for(;;) {
    fsize = sizeof(from);
    cc = recvfrom(socket_fd,&input,sizeof(input),0,(struct sockaddr *)&from,&fsize);
    if (cc < 0) perror("recv_udp:recvfrom"); 
    
    pthread_mutex_lock(&lock);                    // Lock the buffer
    if (full) pthread_cond_wait(&isempty, &lock); // Sleep if buffer occupied
    
    buffer.head = input.head;                     // Copy input into buffer
    buffer.body = input.body; 
    buffer.tail = input.tail;

    full = 1;

    pthread_cond_signal(&gotsome);
    pthread_mutex_unlock(&lock);

  }
}

// Thread to output contents of incoming datagram. Other thread
// gets datagram from network.
void *
outputter()
{
  while (1) {

    pthread_mutex_lock(&lock);
    if (!full) pthread_cond_wait(&gotsome, &lock);

    printf("Got data ::%c%d%c\n", buffer.head, ntohl(buffer.body), buffer.tail);
    fflush(stdout);

    full = 0;
    pthread_cond_signal(&isempty);
    pthread_mutex_unlock(&lock);
  }
}

int
main()
{
  int retcode;
  pthread_t rcvr, outer;
  void *retval;

  retcode = pthread_create (&rcvr, NULL, receiver, NULL);
  if (retcode != 0)
    fprintf (stderr, "create receiver failed %d\n", retcode);
  retcode = pthread_create (&outer, NULL, outputter, NULL);
  if (retcode != 0)
    fprintf (stderr, "create outputter failed %d\n", retcode);
  retcode = pthread_join (rcvr, &retval);
  if (retcode != 0)
    fprintf (stderr, "join receiver failed %d\n", retcode);
  retcode = pthread_join (outer, &retval);
  if (retcode != 0)
    fprintf (stderr, "join outputter failed %d\n", retcode);
  return 0;
}



