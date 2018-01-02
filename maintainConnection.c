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
	}
	printf("\nsend_message(): the variable command has the following value: %s\n", command);


	if(write(sockfd, command, sizeof(command) < 0)){
		perror("\nsend_message(): write error");
		return ERROR;
	}
	printf("\nC: %s\n", command);
	return sockfd;
}

//Will only be used once after performConnection(), after that only maintainConnection()
int maintainConnectionFIRST(int sockfd, int firstServerCommand){
	printf("\nStart method maintainConnectionFIRST()\n");
	printf("\nmaintainConnectionFIRST(): value of firstServerCommand: %i (0=Move,1=Wait,2=Gameover,-1=Error)\n", firstServerCommand);
	if(firstServerCommand == ERROR){
      perror("\nfehlerhaftes firstServerCommand, MAINCONFIRST\n");
      return ERROR;
    }

	 if(firstServerCommand == MOVE){
      printf("\nmaintainConnectionFIRST(): received MOVE from performConnection()\n");
	  if(send_message(sockfd, MOVE) == ERROR){
		  perror("\nmaintainConnectionFIRST(): Error in send_message\n");
		  return ERROR;
	  }
	  else return MOVE;
    }

	if(firstServerCommand == WAIT){
      printf("\nmaintainConnectionFIRST(): received WAIT from performConnection()\n");
       if(send_message(sockfd, WAIT) == ERROR){
		   perror("\nmaintainConnectionFIRST(): Error in send_message\n");
		  return ERROR;
	  }
	  else return WAIT;
    }
	return sockfd;
}


int maintainConnection(int sockfd){
	printf("\nStart method maintainConnection()\n");
	char *serverResponse=malloc(sizeof(char)*MES_LENGTH_SERVER);

	//Hier tritt der Fehler auf, TIMEOUT wird vom Server gelesen
   if((read(sockfd, serverResponse, sizeof(char)*MES_LENGTH_SERVER))<0){
      perror("\nmaintainConnection(): read error\n");
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

short conMOVE(int sockfd){ //TODO
  printf("yay move MAINCON");
  return sockfd;
}
