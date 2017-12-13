CC=gcc
CFLAGS=-g -Wall -Wextra -Werror
ARGS=-g $(GAME_ID) -p  $(PLAYER)

default: build

build: readConfig.c thinkerConnector.c performConnection.c sharedMemory.c draw.c shm_data.h performConnection.h thinkerConnector.h config_header.h drawfield.h
	$(CC) $(CFLAGS) readConfig.c thinkerConnector.c performConnection.c sharedMemory.c draw.c -o sysprak-client

play: build
	./sysprak-client $(ARGS)

clean:
	rm -r sysprak-client
