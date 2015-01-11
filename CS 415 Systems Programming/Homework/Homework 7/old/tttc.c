/************************************************************************
 			         ttt.c
 Simple ttt client. No queries, no timeouts.
 Uses deprecated address translation functions.

 Phil Kearns
 April 12, 1998
 Modified March 2014
************************************************************************/

#include "common.h"

void dump_board();

int main(argc,argv)
int argc; char **argv;
{
  char hostid[128], handle[32], opphandle[32], junk;
  char my_symbol; /* X or O ... specified by server in MATCH message */
  char board[9];
  unsigned short rport, xrport;
  int sock, sfile;
  struct sockaddr_in remote;
  struct hostent *h;
  int num, i, move, valid, finished;
  struct tttmsg inmsg, outmsg;

  if (argc != 1) {
    fprintf(stderr,"ttt:usage is ttt\n");
    exit(1);
  }

  /* Get host,port of server from file. */

  if ( (sfile = open(SFILE, O_RDONLY)) < 0) {
    perror("TTT:sfile");
    exit(1);
  }
  i=0;
  while (1) {
    num = read(sfile, &hostid[i], 1);
    if (num == 1) {
      if (hostid[i] == '\0') break;
      else i++;
    }
    else {
      fprintf(stderr, "ttt:error reading hostname\n");
      exit(1);
    }
  }
  if (read(sfile, &xrport, sizeof(int)) != sizeof(unsigned short)) {
    fprintf(stderr, "ttt:error reading port\n");
      exit(1);
  }
  close(sfile);
  rport = ntohl(xrport);

  /* Got the info. Connect. */

  if ( (sock = socket( AF_INET, SOCK_STREAM, 0 )) < 0 ) {
    perror("ttt:socket");
    exit(1);
  }

  bzero((char *) &remote, sizeof(remote));
  remote.sin_family = AF_INET;
  if ((h = gethostbyname(hostid)) == NULL) {
    perror("ttt:gethostbyname");
    exit(1);
  }
  bcopy((char *)h->h_addr, (char *)&remote.sin_addr, h->h_length);
  remote.sin_port = xrport;
  if ( connect(sock, (struct sockaddr *)&remote, sizeof(remote)) < 0) {
    perror("ttt:connect");
    exit(1);
  }

  /* We're connected to the server. Engage in the prescribed dialog */

  /* Await WHO */

  bzero((char *)&inmsg, sizeof(inmsg));  
  getmsg(sock, &inmsg);
  if (inmsg.type != WHO) protocol_error(WHO, &inmsg);
  
  /* Send HANDLE */

  printf("Enter handle (31 char max):");
  fgets(handle, 31, stdin);
  bzero((char *)&outmsg, sizeof(outmsg));
  outmsg.type = HANDLE;
  strncpy(outmsg.data, handle, 31); outmsg.data[31] = '\0';
  putmsg(sock, &outmsg);

  /* Await MATCH */

  bzero((char *)&inmsg, sizeof(inmsg));  
  getmsg(sock, &inmsg);
  if (inmsg.type != MATCH) protocol_error(MATCH, &inmsg);
  my_symbol = inmsg.board[0];
  strncpy(opphandle, inmsg.data, 31); opphandle[31] = '\0';
  printf("You are playing %c\t your opponent is %s\n\n", my_symbol, opphandle);

  /* In the match */

  for(i=0; i<9; i++) board[i]=' ';
  finished = 0;
  while(!finished){

    /* Await WHATMOVE/RESULT from server */
    
    bzero((char *)&inmsg, sizeof(inmsg));  
    getmsg(sock, &inmsg);
    switch (inmsg.type) {

    case WHATMOVE:
      for(i=0; i<9; i++) board[i]=inmsg.board[i];
      dump_board(stdout,board);
      do {
        valid = 0;
        printf("Enter your move: ");
        num = scanf("%d", &move);
	if (num == EOF) {
	  fprintf(stderr,"ttt:unexpected EOF on standard input\n");
	  exit(1);
	}
	if (num == 0) {
	  if (fread(&junk, 1, 1, stdin)==EOF) {
	    fprintf(stderr,"ttt:unexpected EOF on standard input\n");
	    exit(1);
	  }
	continue;
	}
	if ((num == 1) && (move >= 1) && (move <= 9) ) valid=1;
        if ((valid) && (board[move-1] != ' '))valid=0;
      } while (!valid);
    
      /* Send MOVE to server */

      bzero((char *)&outmsg, sizeof(outmsg));
      outmsg.type = MOVE;
      sprintf(&outmsg.res, "%c", move-1);
      putmsg(sock, &outmsg);
      break;

    case RESULT:
      for(i=0; i<9; i++) board[i]=inmsg.board[i];
      dump_board(stdout,board);
      switch (inmsg.res) {
      case 'W':
	printf("You win\n");
	break;
      case 'L':
	printf("You lose\n");
	break;
      case 'D':
	printf("Draw\n");
	break;
      default:
	fprintf(stderr,"Invalid result code\n");
	exit(1);
      }
      finished = 1;
      break;

    default:
      protocol_error(MOVE);
    }
  }
}


void
dump_board(s,board)
FILE *s;
char board[];
{
  fprintf(s,"%c | %c | %c\n", board[0], board[1], board[2]);
  fprintf(s,"----------\n");
  fprintf(s,"%c | %c | %c\n", board[3], board[4], board[5]);
  fprintf(s,"----------\n");
  fprintf(s,"%c | %c | %c\n", board[6], board[7], board[8]);
}
