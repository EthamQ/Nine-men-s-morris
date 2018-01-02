#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <netdb.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "constants.h"

int send_message(int sockfd, int type){
	printf("\nStart method send_message\n");
	char *command;
	switch(type){
		case MOVE: command = MOVE_MESSAGE; break;
		case WAIT: command = WAIT_MESSAGE; break;
		//TODO add play command,z.b PLAY A1
	}
	printf("\nsend_message(): the variable command has the following value: %s\n", command);

	if(write(sockfd, command, sizeof(command) < 0)){
		perror("send_message(): write error, MAINCON");
		return ERROR;
	}
	printf("\nC: %s\n", command);
	return 0;
}

int maintainConnection(int sockfd){
	//printf("\nStart method maintainConnection()\n");
	char *serverResponse=malloc(sizeof(char)*MES_LENGTH_SERVER);
	printf("\nTestweise übermitteln Spielzug A1");
	write(sockfd, "PLAY A1\n", 15*sizeof(char));
	//Hier tritt der Fehler auf, TIMEOUT wird vom Server gelesen
	if((read(sockfd, serverResponse, sizeof(char)*MES_LENGTH_SERVER))<0){
	  perror("\nmaintainConnection(): read error, MAINCON\n");
		free(serverResponse);
		return ERROR;
	}
  printf("\nmaintainConnection(): Server antwort:\"%s\"\n",serverResponse);

  if(strstr(serverResponse,"+ GAMEOVER")){
	printf("\nmaintainConnection(): received +GAMEOVER from the server\n");
	//TODO: React to GAMEOVER command
    free(serverResponse);
    return GAMEOVER;
  }

	if(strstr(serverResponse,"+ MOVE")){
		printf("\nmaintainConnection(): received +MOVE from the server\n");
		send_message(sockfd, MOVE);
      free(serverResponse);
      return MOVE;
    }

	if(strstr(serverResponse,"+ WAIT")){
		printf("\nmaintainConnection(): received +WAIT from the server\n");
		//TODO: React to WAIT command
		send_message(sockfd, WAIT);
      free(serverResponse);
      return WAIT;
    }
    free(serverResponse);
      return ERROR;
}

//TODO readmessage ?

short conWAIT(int sockfd){
  if((send_message(sockfd, WAIT))==0){
		return 0;
	}
	else{
		return -1;
	}
}

short conGAMEOVER(int sockfd){ //TODO
  return sockfd;
}

short conMOVE(int sockfd){//, char *array){
	printf("Conmove aufgerufen \n");

  if(sockfd == 0){ //Nur temrporaer um compile fhelrer zu verhindern
    printf("nothing");
  }
  return 0;
}

short conPlay(int sockfd, char* move){
	printf("conplay aufgerufen, MAINCON\n");
	if(write(sockfd, move, sizeof(move) < 0)){
		perror("send_message(): write error, MAINCON");
		return ERROR;
	}
	printf("\nC: %s\n", move);
	return 0;
}
