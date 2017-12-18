CC=gcc
CFLAGS=-g -Wall -Wextra
ARGS=-g $(GAME_ID) -p  $(PLAYER)

default: build

build: thinkerConnector.c performConnection.c sharedMemory.c draw.c brain.c readConfig.c
	$(CC) $(CFLAGS) thinkerConnector.c performConnection.c sharedMemory.c draw.c brain.c readConfig.c -o sysprak-client

play: build
	./sysprak-client $(ARGS)

clean:
	rm -r sysprak-client
