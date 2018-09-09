/* refstats.c	Robert Johns	April 6, 2014
 * This program creates three threads, a reader and two lookups, and interweaves them
 * to look up the FQDN for a file of dotted IP addresses.  To speed things up,
 * a cache is implemented which allows the threads to save recently looked-up IP
 * addresses and FQDNs.  Upon completion of lookups for all lines of all files, the 
 * results are printed out in a table, along with the hit ratio of the cache. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stddef.h>
#include <sys/wait.h>
#include <pthread.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#define IPSIZE 32

//TODO: -lpthread in makefile

// node structure for cache and final list
struct node {
	char ipAddr[IPSIZE];
	char FQDN[1024];
	struct node * next;
	struct node * prev;
};

struct linkedList {
	struct node * head;
	struct node * tail;
	struct node * curr;
	int size;
};

// global data
struct timespec filedelay, threaddelay, tim1, tim2;
int numlines, cachesize, nf;
FILE * file;
pthread_mutex_t buff_mutex;
pthread_mutex_t cf_mutex;
int numhits, numread;
pthread_cond_t notempty, notfull;
int reader_done;
struct linkedList * buff, * cache, * final;

int notdone;

// function declarations for threads
void init (void);
static void * lookup (void * arg);
static void * reader (void * arg);
int invalid(char ipa[]);

// debugging function declarations
void printbuff(void);
void printcache(void);

// main funcion
int main (int argc, char *argv[]) {
	
	// local data
	int bflag = 0, Nflag = 0, dflag = 0, Dflag = 0;
	char * bval = NULL;
	char * Nval = NULL;
	char * dval = NULL;
	char * Dval = NULL;
	int ch;
	int i;
	int numFiles = argc - 9;
	int filesRead = 0;
	const char * fileList[numFiles];
	int thread;
	pthread_t readerTh, lookup1Th, lookup2Th;
	void * retVal;
	
	// initialize global data
	init();
	
	// parse and interpret arguments
	if (argc < 10) {
		printf("usage: refstats -b [nl] -N [cs] -d [fd] -D [td] [files]\n");
		exit(-1);
	}
	
	while ((ch = getopt(argc, argv, "b:N:d:D:")) != -1) {
		switch (ch) {
			case 'b':
				bflag = 1;
				bval = optarg;
				break;
			case 'N':
				Nflag = 1;
				Nval = optarg;
				break;
			case 'd':
				dflag = 1;
				dval = optarg;
				break;
			case 'D':
				Dflag = 1;
				Dval = optarg;
				break;
			case '?':
		        printf ("unknown option `-%c'\n", optopt);
			   	exit(-1);
			default:
				break;
		}
	}
	
	i = 0;
	while (bval[i] != '\0') {
		if (!isdigit(bval[i])) {
			printf("error: buffer size must be integer\n");
			exit(-1);
		}
		i++;
	}
	i = 0;
	while (Nval[i] != '\0') {
		if (!isdigit(Nval[i])) {
			printf("error: cache size must be integer\n");
			exit(-1);
		}
		i++;
	}
	i = 0;
	while (dval[i] != '\0') {
		if (!isdigit(dval[i])) {
			printf("error: file delay must be integer\n");
			exit(-1);
		}
		i++;
	}
	i = 0;
	while (Dval[i] != '\0') {
		if (!isdigit(Dval[i])) {
			printf("error: thread delay must be integer\n");
			exit(-1);
		}
		i++;
	}
	//TODO: worry about maximum file length?
	
	if (bflag == 0 || Nflag == 0 || dflag == 0 || Dflag == 0) {
		printf ("usage: refstats -b [nl] -N [cs] -d [fd] -D [td] [files]\n");
		exit(-1);
	}
	
	numlines = atoi(bval);
	if (numlines < 1 || numlines > 1000) {
		printf("error: buffer size must be greater than 0 and less than 1001\n");
		exit (-1);
	}
	cachesize = atoi(Nval);
	if (cachesize < 1 || cachesize > 10000) {
		printf("error: cache size must greater than 0 and less than 10001\n");
		exit(-1);
	}
//NOTE: halved to get away with stuff
	filedelay.tv_nsec   = atol(dval) * 999999;
	threaddelay.tv_nsec = atol(Dval) * 999999;
	
	// set file list
	for (i = 0; i < numFiles; i++) {
		fileList[i] = argv[i + 9];
		if (strlen(fileList[i]) > 4096) {
			printf("error: %dth file name too long\n", i);
			exit(-1);
		}
	}
	nf = numFiles;
	
	// while loop containing threads
	while (filesRead < numFiles) {
		
		notdone = 1;
		reader_done = 0;
		
		pthread_mutex_init (&buff_mutex, NULL);
		pthread_mutex_init (&cf_mutex, NULL);
		pthread_cond_init  (&notfull, NULL);
		pthread_cond_init  (&notempty, NULL);
		
		// open file
		file = fopen(fileList[filesRead], "r");
		if (file == NULL) {
			printf("error: could not find file \n");
			exit (-1);
		}
		
		// create and run threads
		thread = pthread_create (&readerTh, NULL, reader, (void *) "reader");
		if (thread != 0) {
			printf("error creating reader thread\n");
			exit (-1);
		}
		
		thread = pthread_create (&lookup1Th, NULL, lookup, (void *) "lookup1");
		if (thread != 0) {
			printf("error creating lookup thread 1\n");
			exit (-1);
		}
		thread = pthread_create (&lookup2Th, NULL, lookup, (void *) "lookup2");
		if (thread != 0) {
			printf("error creating lookup thread 2\n");
			exit (-1);
		}
		// join threads
		thread = pthread_join (readerTh, &retVal);
		if (thread != 0) {
			printf("error joining reader thread\n");
			exit (-1);
		}
		thread = pthread_join (lookup1Th, &retVal);
		if (thread != 0) {
			printf("error joining lookup1 thread\n");
			exit (-1);
		}
		thread = (pthread_join (lookup2Th, &retVal));
		if (thread != 0) {
			printf("error joining lookup2 thread\n");
			exit(-1);
		}
		
		// move on to next file
		fclose(file);
		filesRead++;
		
	}
	
	printf("**************************************************\n");
	printf("dotted IP address\thost name\n");
	printf("--------------------------------------------------\n");
	final -> curr = final -> head;
	while (final -> curr != NULL) {
		printf("%s\t\t%s\n", final -> curr -> ipAddr, final -> curr -> FQDN);
		final -> curr = final -> curr -> next;
	}
	printf("--------------------------------------------------\n");
	printf("cache hits / attempts:\t%d / %d\n", numhits , numread);
	printf("**************************************************\n");
	
	return 0;
	
}

// reader thread
static void * reader (void * arg) {
	
	char line[IPSIZE];
	struct node * new;
	
	pthread_mutex_lock(&buff_mutex);
	reader_done = 0;
	pthread_mutex_unlock(&buff_mutex);
	
	while (fgets (line, sizeof (line), file) != NULL) {
		
		new = (struct node *) malloc (sizeof (struct node));
		
		if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = '\0';
		
		if (!invalid(line)) {
		
			numread++;
		
			if (nanosleep(&filedelay, &tim1) != 0) {
				printf("error in nanosleep\n");
				exit(-1);
			}
		
			pthread_mutex_lock(&buff_mutex);
			while (buff -> size == numlines) {
				pthread_cond_wait (&notfull, &buff_mutex);
			}
			strcpy(new -> ipAddr, line);
			if (buff -> size == 0) {
				new  -> next = NULL;
				new  -> prev = NULL;
				buff -> head = new;
				buff -> tail = new;
			}
			else {
				new ->  prev = buff -> tail;
				buff -> tail -> next = new;
				buff -> tail = new;
			}
		
			buff -> size++;
		
			pthread_cond_signal  (&notempty);
			pthread_mutex_unlock (&buff_mutex);
		}
	}
	
	pthread_mutex_lock(&buff_mutex);
	reader_done = 1;
	pthread_mutex_unlock(&buff_mutex);
	
	return NULL;
	
}

// lookup thread
static void * lookup(void * arg) {
	
	
	struct node * new;
	struct node * fnode;
	int foundcache;
	int foundfinal;
	int notinfinal;
	
	while (!reader_done || buff -> size > 0) {
		
		
		new   = (struct node *) malloc (sizeof (struct node));
		fnode = (struct node *) malloc (sizeof (struct node));
		new -> next = NULL;
		new -> prev = NULL;
		fnode -> next = NULL;
		fnode -> prev = NULL;
		foundcache = 0;
		foundfinal = 0;
		notinfinal = 0;
		
		// get line from buffer
		pthread_mutex_lock(&buff_mutex);
		if (!notdone) {
			pthread_mutex_unlock(&buff_mutex);
			return NULL;
		}
		while (buff -> size  == 0) {
			if (reader_done) {
				pthread_cond_signal(&notempty);
				notdone = 0;
				pthread_mutex_unlock(&buff_mutex);
				return NULL;
			}
			pthread_cond_wait(&notempty, &buff_mutex);
		}
		if (buff -> head != NULL) {
			buff -> head -> prev = NULL;
			strcpy(new -> ipAddr, buff -> head -> ipAddr);
			buff -> head = buff -> head -> next;
		}
		else {
			pthread_mutex_unlock(&buff_mutex);
			return NULL;
		}
		buff -> size--;
		if (reader_done == 1 && buff -> size == 0) {
			notdone = 0;
			pthread_mutex_unlock(&buff_mutex);
		}
		pthread_cond_signal(&notfull);
		pthread_mutex_unlock(&buff_mutex);
		
		if (nanosleep(&threaddelay, &tim2) != 0) {
			printf("error in nanosleep\n");
			exit(-1);
		}
		
		// check cache
		pthread_mutex_lock(&cf_mutex);		
		cache -> curr = cache -> head;
		final -> curr = final -> head;
		while (cache -> curr != NULL && !foundcache) {
			if (strcmp(cache -> curr -> ipAddr, new -> ipAddr) != 0) {
				cache -> curr = cache -> curr -> next;
			}
			else {
				foundcache = 1;
			}
		}
		// if hit
		if (foundcache) {
			if (strcmp(cache -> curr -> ipAddr, cache -> head -> ipAddr) == 0) {
				numhits++;
			}
			else if (strcmp(cache -> curr -> ipAddr, cache -> tail -> ipAddr) == 0) {
				cache -> curr -> next = cache -> head;
				cache -> head -> prev = cache -> curr;
				cache -> head = cache -> curr;
				cache -> tail = cache -> tail -> prev;
				cache -> tail -> next = NULL;
				numhits++;
			}
			else {
				cache -> curr -> prev -> next = cache -> curr -> next;
				cache -> curr -> next -> prev = cache -> curr -> prev;
				cache -> head -> prev = cache -> curr;
				cache -> curr -> next = cache -> head;
				cache -> head = cache -> curr;
				numhits++;
			}
			strcpy(new -> ipAddr, cache -> curr -> ipAddr);
			strcpy(new -> FQDN, cache -> curr -> FQDN);
			if (cache -> size > cachesize) {
				cache -> tail -> prev -> next = NULL;
				cache -> tail = cache -> tail -> prev;
			}
		}
		// if miss
		else {
			struct hostent *hp;
			in_addr_t adr_clnt = inet_addr(new -> ipAddr);
			hp = gethostbyaddr((char *)&adr_clnt, sizeof(adr_clnt), AF_INET);
			if (hp != NULL) {
				strcpy(new -> FQDN, hp -> h_name);
			}
			else {
				strcpy(new -> FQDN, "");
			}
			if (cache -> size == 0) {
				cache -> head = new;
				cache -> tail = new;
			}
			else if (cache -> size == 1) {
				new -> next = cache -> head;
				new -> next -> prev = new;
				cache -> tail = new -> next;
				cache -> head = new;
			}
			else {
				new -> next = cache -> head;
				cache -> head -> prev = new;
				cache -> head = new;
			}
			cache -> size ++;
			if (cache -> size > cachesize) {
				cache -> tail -> prev -> next = NULL;
				cache -> tail = cache -> tail -> prev;
				cache -> size --;
			}
		}
		
		// add to final list
		strcpy(fnode -> ipAddr, new -> ipAddr);
		strcpy(fnode -> FQDN, new -> FQDN);
		while (final -> curr != NULL && !foundfinal && !notinfinal) {
			if (strcmp(fnode -> FQDN, final -> curr -> FQDN) == 0) {
				if (strcmp(fnode -> FQDN, "") == 0) {
					while (strcmp(fnode -> FQDN, "") == 0 && !foundfinal && !notinfinal){
						if (strcmp(fnode -> ipAddr, final -> curr -> ipAddr) == 0) {
							foundfinal = 1;
						}
						else if (strcmp(fnode -> ipAddr, final -> curr -> FQDN) > 0) {
							final -> curr = final -> curr -> next;
						}
						else {
							notinfinal = 1;
						}
					}
				}
				else {
					foundfinal = 1;
				}
			}
			else if (strcmp(fnode -> FQDN, final -> curr -> FQDN) > 0) {
				final -> curr = final -> curr -> next;
			}
			else {
				notinfinal = 1;
			}
		}
		if (notinfinal) {
			if (final -> curr == final -> head) {
				fnode -> next = final -> head;
				fnode -> prev = NULL;
				final -> head -> prev = fnode;
				final -> head = fnode;
			}
			else {
				fnode -> next = final -> curr;
				fnode -> prev = final -> curr -> prev;
				final -> curr -> prev -> next = fnode;
				final -> curr -> prev = fnode;
			}
		}
		else if (!foundfinal){
			fnode -> next = NULL;
			fnode -> prev = final -> tail;
			if (final -> tail != NULL) final -> tail -> next = fnode;
			else final -> head = fnode;
			final -> tail = fnode;
		}
		pthread_mutex_unlock(&cf_mutex);
	}
	
	
	pthread_mutex_lock(&buff_mutex);
	pthread_cond_signal(&notempty);
	pthread_mutex_unlock(&buff_mutex);

	return NULL;
}

// initialize global data- removed from main for code cleanliness
void init (void) {

	reader_done = 0;
	
	buff  = (struct linkedList *) malloc (sizeof (struct linkedList));
	cache = (struct linkedList *) malloc (sizeof (struct linkedList));
	final = (struct linkedList *) malloc (sizeof (struct linkedList));
	
	buff  -> size = 0;
	cache -> size = 0;
	final -> size = 0;
	
	buff -> head = NULL;
	buff -> curr = NULL;
	buff -> tail = NULL;

	cache -> head = NULL;
	cache -> curr = NULL;
	cache -> tail = NULL;

	final -> head = NULL;
	final -> curr = NULL;
	final -> tail = NULL;
	
	notdone = 1;
		
	pthread_mutex_init (&buff_mutex, NULL);
	pthread_mutex_init (&cf_mutex, NULL);
	pthread_cond_init  (&notfull, NULL);
	pthread_cond_init  (&notempty, NULL);

}

// function to tell if an ip address is invalid
int invalid (char ipa[]) {
	
	int count = 0;
	int flag = 0;
	int timesince = 0;
	int i = 0;
	while(ipa[i] != '\0') {
		if (ipa[i] == '.') {
			count++;
			timesince = 0;
		}
		else if (isdigit(ipa[i])) {
			timesince++;
		}
		else {
			flag = 1;
		}
		if(timesince == 4) flag = 1;
		i++;
	}
	if (count != 3) flag = 1;
	
	return flag;
}