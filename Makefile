CC=gcc
CFLAGS=-g -Wall -Wextra -Werror
ARGS=-g $(GAME_ID) -p  $(PLAYER)

default: build

build: sysprak-client.c performConnection.c
	$(CC) $(CFLAGS) sysprak-client.c performConnection.c -o sysprak-client

play: build
	./sysprak-client $(ARGS)

clean:
	rm -r sysprak-client
