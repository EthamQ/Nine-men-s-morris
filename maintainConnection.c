#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <netdb.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define BUF 256
#define MES_LENGTH_SERVER 1048
#define MES_LENGTH_CLIENT 200
#define ATTEMPTS_INVALID 20
#define ERROR -1
#define MOVE 0
#define WAIT 1
#define WAIT_MESSAGE "OKWAIT\n"
#define MOVE_MESSAGE "THINKING\n"
#define ATTEMPTS_INVALID 20


//static char messageToSend[1048]; //= ""; //test, "" spaeter entfernen ??


//Will only be used once after performConnection(), after that only maintainConnection()
int maintainConnectionFIRST(int sockfd, int firstServerCommand){
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
    }

	if(firstServerCommand == WAIT){
      printf("\nmaintainConnectionFIRST(): received WAIT from performConnection()\n");
       if(send_message(sockfd, WAIT) == ERROR){
		   perror("\nmaintainConnectionFIRST(): Error in send_message\n");
		  return ERROR;
	  }
    }
	return sockfd;
}

//TODO: adjust return values
short maintainConnection(int sockfd){
	char *serverResponse=malloc(sizeof(char)*MES_LENGTH_SERVER);

   if((read(sockfd, serverResponse, sizeof(serverResponse)))<0){
      perror("\nmaintainConnection(): read error");
	  free(serverResponse);
	  return -1;
   }
   printf("maintainConnection(): Server antwort:\"%s\"\n",serverResponse);

    if(strstr(serverResponse,"+GAMEOVER")){
		printf("\nmaintainConnection(): received +GAMEOVER from the server\n");
		//TODO: React to GAMEOVER command
      free(serverResponse);
      return 0;
    }

	if(strstr(serverResponse,"+MOVE")){
		printf("\nmaintainConnection(): received +MOVE from the server\n");
		//TODO: React to MOVE command
		send_message(sockfd, MOVE)
      free(serverResponse);
      return 0;
    }

	if(strstr(serverResponse,"+WAIT")){
		printf("\nmaintainConnection(): received +WAIT from the server\n");
		//TODO: React to WAIT command
		send_message(sockfd, WAIT)
      free(serverResponse);
      return 0;
    }
    free(serverResponse);
      return sockfd;
}


int send_message(int sockfd, int type){
	char *command;
	switch(type){
		case MOVE: command = MOVE_MESSAGE; break;
		case WAIT: command = WAIT_MESSAGE; break;
	}

	if(write(sockfd, command, sizeof(command) < 0){
		perror("\nsend_message(): write error");
		return ERROR;
	}
	return sockfd;
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
