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

/*
//ACHTUNG: MESSAGE VORHER IN messageToSend SCHREIBEN !
int sendConMess(int sockfd, char* messageToSend){
  const char* uselessChar;
  uselessChar = "";
  short attempts = 0;
  short testifvalid = -1; //TODO deklaration dieser Variable verschieben ?
  //char messageToSend = message;
  if(strcmp(messageToSend, uselessChar) != 0){
    //Siehe perfCon
    while(testifvalid < 0){
      testifvalid = write(sockfd, messageToSend, (int)strlen(messageToSend));
      attempts++;
      if(attempts >= ATTEMPTS_INVALID){
          //messageToSend = "";
          return -1;
      }
    }
  }
  else{
    perror("messageToSend == 0, MAINCON");
  }
  printf("Wir senden: \"%s\" MAINCON\n", messageToSend);
  //messageToSend = "";
  return 0; //TODO wegtun
}


char* readConMess(int sockfd){
  char *messageToRead;
  char messageBuffer[MES_LENGTH_SERVER];

  messageToRead=malloc(sizeof(messageToRead)); //messageToRead ???
  short attempts = 0;
  short testifvalid = -1;

  while(testifvalid < 0){
    testifvalid = read(sockfd, messageBuffer, MES_LENGTH_SERVER);
    attempts++;
    if(attempts >= ATTEMPTS_INVALID){
      perror("Invalid server response");
      printf("%sfehlertest",messageBuffer);
      messageToRead =  "ERROR";
      return messageToRead;
    }
  }
  printf("Server sendet: \"%s\" MAINCON\n", messageBuffer);
  strcpy(messageToRead,messageBuffer);
  return messageToRead;
  //TODO MEMORYLEAK VERHINDERN
}

short conWAIT(int sockfd){
  sendConMess(sockfd, WAIT_MESSAGE);
  return sockfd;
}


short conGAMEOVER(int sockfd){
  return sockfd;
}

short conMOVE(int sockfd){
  printf("yay move MAINCON");
  //messageToSend = "OKWAIT\n";
  char* messageCon = (char*)malloc(sizeof(char)*BUF);
  messageCon = "OKWAIT\n";
  if(sendConMess(sockfd, messageCon)<0){
    perror("conMove fehler, MAINCON");
  }
  free(messageCon);
  return sockfd;
}
*/


/*
altes conwait
char *clientResponse = (char*)malloc(sizeof(char)*BUF);
//strcpy(clientResponse, ""); //DEFAULT WERT
strcpy(clientResponse, "OKWAIT");
strcat(clientResponse, "\n");

int n = write(sockfd,clientResponse,sizeof(clientResponse));
if(n<0){
  perror("Fehler bei Schreiben +OKWAIT in Sockfd, maintainConnection.c");
  return -1;
}
return 0;
*/
