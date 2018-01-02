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
	printf("Start method maintainConnectionFIRST()");
	if(firstServerCommand == ERROR){
	char *command;
	switch(type){
		case MOVE: command = MOVE_MESSAGE; break;
		case WAIT: command = WAIT_MESSAGE; break;
	}
	printf("send_message(): the variable command has the following value: %s", command);

	
	if(write(sockfd, command, sizeof(command) < 0)){
		perror("\nsend_message(): write error");
		return ERROR;
	}
	return sockfd;
}

//Will only be used once after performConnection(), after that only maintainConnection()
int maintainConnectionFIRST(int sockfd, int firstServerCommand){
	printf("Start method maintainConnectionFIRST()");
	if(firstServerCommand == ERROR){
      perror("\nmaintainConnectionFIRST(): Error in performConnection\n");
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
	printf("Start method maintainConnection()");
	char *serverResponse=malloc(sizeof(char)*MES_LENGTH_SERVER);

   if((read(sockfd, serverResponse, sizeof(serverResponse)))<0){
      perror("\nmaintainConnection(): read error");
	  free(serverResponse);
	  return ERROR;
   }
   printf("maintainConnection(): Server antwort:\"%s\"\n",serverResponse);

    if(strstr(serverResponse,"+GAMEOVER")){
		printf("\nmaintainConnection(): received +GAMEOVER from the server\n");
		//TODO: React to GAMEOVER command
      free(serverResponse);
      return GAMEOVER;
    }

	if(strstr(serverResponse,"+MOVE")){
		printf("\nmaintainConnection(): received +MOVE from the server\n");
		send_message(sockfd, MOVE);
      free(serverResponse);
      return MOVE;
    }

	if(strstr(serverResponse,"+WAIT")){
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
