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

char thinkingPRC []= "THINKING\n";



int maintainConnection(int sockfd, struct SHM_data* shm_pointer){
	//printf("\nStart method maintainConnection()\n");
	char *serverResponse=malloc(sizeof(char)*MES_LENGTH_SERVER);
	
	
	if((read(sockfd, serverResponse, sizeof(char)*MES_LENGTH_SERVER))<0){
	  perror("\nmaintainConnection(): read error, MAINCON\n");
		free(serverResponse);
		return ERROR;
	}
  //printf("%s",serverResponse);
  printf("\nmaintainConnection():\nS: %s",serverResponse);
  //printf("größe der nachricht: %i\n",sizeof(char)*MES_LENGTH_SERVER);

 

  

	
  	if(strstr(serverResponse,"+ MOVE 3000")){
		//if(strcmp(serverResponse, "+ MOVE\n")==0){
		printf("\nmaintainConnection(): received +MOVE from the server\n");
		
		if(write(sockfd, thinkingPRC, (int)strlen(thinkingPRC)) <0){
			perror("Fehler beim senden von THINKING");
		}
		
		printf("C: %s", thinkingPRC);
		
		if(read(sockfd, serverResponse, sizeof(char)*MES_LENGTH_SERVER) < 0){
			perror("Fehler beim empfangen von OKTHINK");
		};

		printf("\nS: %s\n",serverResponse);
		
		read_piecelist(shm_pointer, serverResponse);
      free(serverResponse);
      return MOVE;
    }
	
	 if(strcmp(serverResponse, "+ MOVEOK\n")==0){
	//if(strstr(serverResponse,"+ MOVEOK")){
		printf("maintainConnection(): received + MOVEOK from the server\n");
      free(serverResponse);
      return MOVEOK;
    }
  

	//if(strcmp(serverResponse, "+ WAIT\n")==0){
	if(strstr(serverResponse,"+ WAIT")){
		printf("\nmaintainConnection(): received +WAIT from the server\n");
      free(serverResponse);
      return WAIT;
    }
	
	if(strstr(serverResponse,"OKTHINK")){
		//if(strcmp(serverResponse, "+ MOVE\n")==0){
		printf("\nmaintainConnection(): received +OKTHINK from the server\n");
      free(serverResponse);
      return OKTHINK;
    }
	
			if(strstr(serverResponse,"CAPTURE") && strstr(serverResponse,"+ MOVE")){
    free(serverResponse);
    return CAPTURE;
  }
	
  if(strstr(serverResponse,"+ GAMEOVER")){
	printf("\nmaintainConnection(): received +GAMEOVER from the server\n");
    free(serverResponse);
    return GAMEOVER;
  }
  
	
    free(serverResponse);
      return ERROR;
}


short send_move_to_server(int sockfd, char* move){
	printf("conplay aufgerufen, MAINCON\n");
	
	if(write(sockfd, move, sizeof(move)) < 0){
		perror("write error, MAINCON");
		return ERROR;
	}
	printf("\nC: %s\n", move);
	return 0;
}
