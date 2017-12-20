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

short conWAIT(int sockfd){
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
}

short conGAMEOVER(int sockfd){
  if(sockfd == 0){ //Nur temrporaer um compile fehler zu verhindern
    printf("nothing");
  }
  return 0;
}

//short conMOVE(int sockfd, char *array){
 // if(sockfd == 0){ //Nur temrporaer um compile fhelrer zu verhindern
  //  printf("nothing");
  //}
 // return 0;
//}
