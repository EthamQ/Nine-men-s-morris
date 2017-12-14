CC=gcc
CFLAGS=-g -Wall -Wextra -Werror
ARGS=-g $(GAME_ID) -p  $(PLAYER)

default: build

build: thinkerConnector_WORKING.c performConnection_WORKING.c sharedMemory.c draw.c brain.c readConfig.c
	$(CC) $(CFLAGS) thinkerConnector_WORKING.c performConnection_WORKING.c sharedMemory.c draw.c brain.c readConfig.c -o sysprak-client

play: build
	./sysprak-client $(ARGS)

clean:
	rm -r sysprak-client
