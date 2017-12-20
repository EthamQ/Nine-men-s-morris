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

static char messageToSend[1048]; //siehe perfcon

short maintainConnection(int sockfd){
  char *serverResponse=malloc(sizeof(char)*BUF);

   if((read(sockfd,serverResponse,sizeof(serverResponse)))<0){
      perror("read bei maintainConnection");
    }
    printf("Server antwort, maintainConnection %s\n",serverResponse);

    if(strstr(serverResponse,"+WAIT")){
      return 0;
    }
    if(strstr(serverResponse,"+GAMEOVER")){
      return 1;
    }
    if(strstr(serverResponse,"+MOVE")){
      return 2;
    }
    return -1;
}

//ACHTUNG: MESSAGE VORHER IN messageToSend SCHREIBEN !
int sendConMess(int sockfd){
  short attempts = 0;
  short testifvalid = -1 //TODO deklaration dieser Variable verschieben ?
  if(messageToSend != ""){
    //Siehe perfCon
    while(testifvalid < 0){
      testifvalid = write(sockfd, messageToSend, (int)strlen(messageToSend));
      attempts++;
      if(attempts >= ATTEMPTS_INVALID){
          messageToSend = "";
          return -1;
      }
    }
  }
  printf("Wir senden: \"%s\" MAINCON\n", messageToSend);
  messageToSend = "";
  return 0;
}

char* readConMess(int sockfd){
  char *messageToRead;
  char messageBuffer[MES_LENGTH_SERVER];

  messageToRead=malloc(sizeof(messageToRead));
  short attempts = 0;
  short testifvalid = -1

  while(testifvalid < 0){
    testifvalid = read(sockfd, messageBuffer, MES_LENGTH_SERVER);
    attempts++;
    if(attempts >= ATTEMPTS_INVALID){
      perror("Invalid server response2");
      printf("%sfehlertest",messageBuffer);
      return -1;
    }
  }
  printf("Server sendet: \"%s\" MAINCON\n", messageBuffer);
  strcpy(move,messageBuffer);
  return messageToRead;
  //TODO MEMORYLEAK VERHINDERN
}

short conWAIT(int sockfd){
  /*
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
  messageToSend = "OKWAIT\n";
  sendConMess(sockfd);
  return 0;
}

short conGAMEOVER(int sockfd){
  if(sockfd < 0){
    perror("Fehlerhafter Sockfd in gameover, MAINCON");
  }
}

short conMOVE(int sockfd){//, char *array){

  return 0;
}
