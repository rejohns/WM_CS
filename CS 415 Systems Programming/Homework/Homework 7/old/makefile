all: TTT tttc

TTT: TTT.c
	gcc -Wall -o TTT msg.c TTT.c

tttc: tttc.c
	gcc -Wall -o tttc child.c msg.c tttc.c

clean:
	rm -f TTT tttc