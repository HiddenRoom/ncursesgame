CC=gcc

all: Engine

Engine: main.c
	$(CC) -o Engine main.c -lncurses

clean:
	shred -u Engine
