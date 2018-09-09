all: TTT tttc

TTT: TTT.c
	gcc -o TTT msg.c TTT.c

tttc: tttc.c
	gcc -o tttc child.c msg.c tttc.c

clean:
	rm -f TTT tttc
