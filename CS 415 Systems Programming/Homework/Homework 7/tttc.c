/************************************************************************
 			         ttt.c
 Simple ttt client. No queries, no timeouts.
 Uses deprecated address translation functions.

 Phil Kearns
 April 12, 1998
 Modified March 2014

 Modified further by
 Robert Johns	-	April 25, 2014

 extends ttt client to gui, adds resignation capabilities. communicates
 with gui through pipes to child process "child" running wish. 

************************************************************************/

#include "common.h"
#include "child.h"

void draw_win();
void dump_board();

int main(argc,argv)
int argc; char **argv;
{
  char hostid[128], handle[32], opphandle[32], junk;
  char my_symbol, opp_symbol; /* X or O ... specified by server in MATCH message */
  char board[9];
  unsigned short rport, xrport;
  int sock, sfile;
  struct sockaddr_in remote;
  struct hostent *h;
  int num, i, move, valid, finished;
  struct tttmsg inmsg, outmsg;

	int childpid;
	FILE *read_from, *write_to;

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

  printf("enter handle (31 char max): ");
  fgets(handle, 31, stdin);
  bzero((char *)&outmsg, sizeof(outmsg));
  outmsg.type = HANDLE;
  strncpy(outmsg.data, handle, 31); outmsg.data[31] = '\0';
  putmsg(sock, &outmsg);

	childpid = start_child("wish", &read_from, &write_to);
	fprintf(write_to, "source gui.tcl\n");
	fprintf(write_to, ".i itemconfigure player -text \"you are:     %s\"\n", handle);


  /* Await MATCH */
	fprintf(write_to, ".i itemconfigure status -text \"status:       awaiting match\"\n");
  bzero((char *)&inmsg, sizeof(inmsg));  
  getmsg(sock, &inmsg);
  if (inmsg.type != MATCH) protocol_error(MATCH, &inmsg);
  my_symbol = inmsg.board[0];
	if (my_symbol == 'X') opp_symbol = 'O';
	else opp_symbol = 'X';
  strncpy(opphandle, inmsg.data, 31); opphandle[31] = '\0';
	fprintf(write_to,\
	 ".i itemconfigure player -text \"you are:     (%c) %s\"\n", my_symbol, handle);
	fprintf(write_to,
	 ".i itemconfigure opp -text \"opponent: (%c) %s\"\n", opp_symbol, opphandle);

  /* In the match */

  for(i=0; i<9; i++) board[i]=' ';
  finished = 0;
  while(!finished){

    /* Await WHATMOVE/RESULT from server */
    fprintf(write_to,\
		 ".i itemconfigure status -text \"status:       awaiting opponent move\"\n");
    bzero((char *)&inmsg, sizeof(inmsg));  
    getmsg(sock, &inmsg);
    switch (inmsg.type) {

		// enter a move
    case WHATMOVE:
			fprintf(write_to, "if {$silent == 0} {bell}\n");
			fprintf(write_to, "set turn 1\n");
			fprintf(write_to, ".i itemconfigure status -text \"status:       your move\"\n");
	    for(i=0; i<9; i++) board[i]=inmsg.board[i];
			dump_board(write_to, board);
	    do {
	    	valid = 0;
				fscanf(read_from, "%d", &move);
				if (move >= 1 && move <= 9 && board[move - 1] == ' ') {
					valid = 1;
					fprintf(write_to, "set turn 0\n");
					board[move - 1] = my_symbol;
					dump_board(write_to, board);
				}
				else if (move >= 1 && move <= 9 && board[move - 1] != ' ') {
					fprintf(write_to, "if {$silent == 0} {bell}\n");
				}
				else if (move == 0) {
					valid = 1;
					fprintf(write_to, "set done 1\n");
					fprintf(write_to,\
					 ".i itemconfigure status -text \"status:       YOU RESIGNED\"\n");
				}
				else {
					printf("something's wrong\n");
					exit(-1);
				}

	 	  } while (!valid);
 	   
			/* Send MOVE to server */
			if (move != 0) {
  	  	bzero((char *)&outmsg, sizeof(outmsg));
  	  	outmsg.type = MOVE;
  	  	sprintf(&outmsg.res, "%c", move-1);
  	  	putmsg(sock, &outmsg);
	    	break;
			}
			else {
				bzero((char *) &outmsg, sizeof(outmsg));
				outmsg.type = RESIGN;
				putmsg(sock, &outmsg);
				finished = 1;
				break;
			}

    case RESULT:
      for(i=0; i<9; i++) board[i]=inmsg.board[i];
      switch (inmsg.res) {
      case 'W':
				fprintf(write_to, ".i itemconfigure status -text \"status:       YOU WIN\"\n");
				fprintf(write_to, "set done 1\n");
				dump_board(write_to, board);
				draw_win(write_to, board);
				break;
      case 'L':
				fprintf(write_to, ".i itemconfigure status -text \"status:       YOU LOSE\"\n");
				fprintf(write_to, "set done 1\n");
				dump_board(write_to, board);
				draw_win(write_to, board);
				break;
      case 'D':
				fprintf(write_to, ".i itemconfigure status -text \"status:       A DRAW\"\n");
				fprintf(write_to, "set done 1\n");
				dump_board(write_to, board);
				break;
			case 'R':
					fprintf(write_to,\
					 ".i itemconfigure status -text \"status:       YOUR OPPONENT RESIGNED\"\n");
					fprintf(write_to, "set done 1\n");
					dump_board(write_to, board);
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

void draw_win(FILE *s, char board[]) {
	int set;
	if (board[0] == board[1] && board[1] == board[2] && board[0] != ' ') {
		fprintf(s, ".c create line 49 49 249 49 -width 2 -tags win\n");
		set = 0;
	}
	else if (board[3] == board[4] && board[4] == board[5] && board[3] != ' ') {
		fprintf(s, ".c create line 49 149 249 149  -width 2 -tags win\n");
		set = 3;
	}
	else if (board[6] == board[7] && board[7] == board[8] && board[6] != ' ') {
		fprintf(s, ".c create line 49 249 249 249  -width 2 -tags win\n");
		set = 6;
	}
	else if (board[0] == board[3] && board[3] == board[6] && board[0] != ' ') {
		fprintf(s, ".c create line 49 49 49 249  -width 2 -tags win\n");
		set = 0;
	}
	else if (board[1] == board[4] && board[4] == board[7] && board[1] != ' ') {
		fprintf(s, ".c create line 149 49 149 249  -width 2 -tags win\n");
		set = 1;
	}
	else if (board[2] == board[5] && board[5] == board[8] && board[2] != ' ') {
		fprintf(s, ".c create line 249 49 249 249  -width 2 -tags win\n");
		set = 2;
	}
	else if (board[0] == board[4] && board[4] == board[8] && board[0] != ' ') {
		fprintf(s, ".c create line 49 49 249 249  -width 2 -tags win\n");
		set = 0;
	}
	else if (board[2] == board[4] && board[4] == board[6] && board[2] != ' ') {
		fprintf(s, ".c create line 249 49 49 249  -width 2 -tags win\n");
		set = 2;
	}
	else {
		printf("error: not a winning board\n");
		exit(-1);
	}
	if (board[set] == 'X') fprintf(s, ".c itemconfigure win -fill red\n");
	else fprintf(s, ".c itemconfigure win -fill blue\n");
}

void
dump_board(FILE *s, char board[]) {
  fprintf(s,".c itemconfigure one -text %c\n", board[0]);
  fprintf(s,".c itemconfigure two -text %c\n", board[1]);
  fprintf(s,".c itemconfigure three -text %c\n", board[2]);
  fprintf(s,".c itemconfigure four -text %c\n", board[3]);
  fprintf(s,".c itemconfigure five -text %c\n", board[4]);
  fprintf(s,".c itemconfigure six -text %c\n", board[5]);
  fprintf(s,".c itemconfigure seven -text %c\n", board[6]);
  fprintf(s,".c itemconfigure eight -text %c\n", board[7]);
  fprintf(s,".c itemconfigure nine -text %c\n", board[8]);
}
