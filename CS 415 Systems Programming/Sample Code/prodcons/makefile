CC=gcc
CFLAGS=-g -Wall -D_REENTRANT
LIBS=-lpthread

prodcons: prodcons.c
	$(CC) $(CFLAGS) -o prodcons prodcons.c $(LIBS)

clean:
	rm -f prodcons core.*

