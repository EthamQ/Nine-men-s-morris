CC=gcc
CFLAGS=-g -Wall -Wextra -Werror -pthread
ARGS=-g $(GAME_ID) -p $(PLAYER) -c $(CONFIG)

default: build

build: thinkerConnector.c performConnection.c maintainConnection.c draw.c brain.c readConfig.c readPieceList.c
	$(CC) $(CFLAGS) thinkerConnector.c performConnection.c maintainConnection.c draw.c brain.c readConfig.c readPieceList.c -o sysprak-client

play: build 
	./sysprak-client $(ARGS)


clean:
	rm -r -f sysprak-client

val:
	valgrind --leak-check=full --trace-children=yes --track-origins=yes ./sysprak-client
