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
#define ATTEMPTS_INVALID 20


//static char messageToSend[1048]; //= ""; //test, "" spaeter entfernen ??

short maintainConnection(int sockfd){
  char *serverResponse=malloc(sizeof(char)*BUF);

   if((read(sockfd,serverResponse,sizeof(serverResponse)))<0){
      perror("read bei maintainConnection");
    }
    printf("Server antwort, maintainConnection %s\n",serverResponse);

    if(strstr(serverResponse,"+WAIT")){
      free(serverResponse);
      return 0;
    }
    if(strstr(serverResponse,"+GAMEOVER")){
      free(serverResponse);
      return 1;
    }
    if(strstr(serverResponse,"+MOVE")){
      free(serverResponse);
      return 2;
    }
    free(serverResponse);
    return -1;
}

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

  //messageToSend = "OKWAIT\n";
  char* messageCon = (char*)malloc(sizeof(char)*BUF);
  messageCon = "OKWAIT\n";
  sendConMess(sockfd, messageCon);
  free(messageCon);
  return sockfd;
}

short conGAMEOVER(int sockfd){
  return sockfd;
}

short conMOVE(int sockfd){

  return sockfd;
}


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
