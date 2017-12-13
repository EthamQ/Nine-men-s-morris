CC=gcc
CFLAGS=-g -Wall -Wextra -Werror
ARGS=-g $(GAME_ID) -p  $(PLAYER)

default: build

<<<<<<< HEAD
build: thinkerConnector.c performConnection.c brain.c
	$(CC) $(CFLAGS) thinkerConnector.c performConnection.c brain.c -o sysprak-client
=======
build: thinkerConnector_WORKING.c performConnection_WORKING.c sharedMemory.c draw.c
	$(CC) $(CFLAGS) thinkerConnector_WORKING.c performConnection_WORKING.c sharedMemory.c draw.c -o sysprak-client
>>>>>>> 9878ab0d973ba94636da88daa0ec2461747de510

play: build
	./sysprak-client $(ARGS)

clean:
	rm -r sysprak-client
