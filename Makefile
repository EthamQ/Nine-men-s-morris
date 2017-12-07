CC=gcc
CFLAGS=-g -Wall -Wextra -Werror
ARGS=-g $(GAME_ID) -p  $(PLAYER)

default: build

build: thinkerConnector.c performConnection.c sharedMemory.c draw.c
	$(CC) $(CFLAGS) thinkerConnector.c performConnection.c sharedMemory.c draw.c -o sysprak-client

play: build
	./sysprak-client $(ARGS)

clean:
	rm -r sysprak-client
