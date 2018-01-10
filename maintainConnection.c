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
#include "shm_data.h"

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

int maintainConnection(int sockfd, struct SHM_data* shm_pointer){
	//printf("\nStart method maintainConnection()\n");
	char *serverResponse=malloc(sizeof(char)*MES_LENGTH_SERVER);
	
	
	if((read(sockfd, serverResponse, sizeof(char)*MES_LENGTH_SERVER))<0){
	  perror("\nmaintainConnection(): read error, MAINCON\n");
		free(serverResponse);
		return ERROR;
	}
  printf("%s",serverResponse);
  printf("\nmaintainConnection():\nS: %s",serverResponse);
  //printf("größe der nachricht: %i\n",sizeof(char)*MES_LENGTH_SERVER);

 
  
  if(strstr(serverResponse,"+ GAMEOVER")){
	printf("\nmaintainConnection(): received +GAMEOVER from the server\n");
    free(serverResponse);
    return GAMEOVER;
  }
  
   if(strstr(serverResponse,"CAPTURE")){
	printf("\nmaintainConnection(): received CAPTURE from the server\n");
    free(serverResponse);
    return CAPTURE;
  }

  if(strcmp(serverResponse, "+ MOVEOK\n")==0){
	//if(strstr(serverResponse,"+ MOVEOK")){
		printf("\nmaintainConnection(): received + MOVEOK from the server\n");
      free(serverResponse);
      return MOVEOK;
    }
	
	if(strstr(serverResponse,"+ MOVE")){
		//if(strcmp(serverResponse, "+ MOVE\n")==0){
		printf("\nmaintainConnection(): received +MOVE from the server\n");
		//send_message(sockfd, MOVE);
		read_piecelist(shm_pointer, serverResponse);
      free(serverResponse);
      return MOVE;
    }

	//if(strcmp(serverResponse, "+ WAIT\n")==0){
	if(strstr(serverResponse,"+ WAIT")){
		printf("\nmaintainConnection(): received +WAIT from the server\n");
		//TODO: React to WAIT command
		//send_message(sockfd, WAIT);
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

	int testplay(int sockfd){
		//char *move;
		char *move= "PLAY A1\n";
	printf("\nGIB EINEN SPIELZUG EIN: ");
	//scanf("%s", move);
	//fgets(move,20,stdin); 
	//printf("\nIn move steht: %s", move);
	//printf("Testzeile um zu sehen ob ein Zeilenumbruch da ist");
	write(sockfd, move, sizeof(move));
		return 0;
	}
short send_move_to_server(int sockfd, char* move){
	printf("conplay aufgerufen, MAINCON\n");
	
	if(write(sockfd, move, sizeof(move)) < 0){
		perror("send_message(): write error, MAINCON");
		return ERROR;
	}
	printf("\nC: %s\n", move);
	return 0;
}
