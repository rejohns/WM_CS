CC=gcc
CFLAGS=-g -Wall

all: inet_rstream inet_wstream

inet_rstream: inet_rstream.o
	$(CC) -o inet_rstream inet_rstream.o

inet_wstream: inet_wstream.o
	$(CC) -o inet_wstream inet_wstream.o

clean:
	rm -f *.o inet_rstream inet_wstream