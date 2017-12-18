#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <netdb.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>


short maintainConnection(int sockfd){
  if(sockfd == 0){ //Nur temrporaer um compile fhelrer zu verhindern
    printf("nothing");
  }
  return 0;
}

short conWAIT(int sockfd){
  if(sockfd == 0){ //Nur temrporaer um compile fhelrer zu verhindern
    printf("nothing");
  }
  return 0;
}

short conGAMEOVER(int sockfd){
  if(sockfd == 0){ //Nur temrporaer um compile fhelrer zu verhindern
    printf("nothing");
  }

  return 0;
}

short conMOVE(int sockfd, char *array){
  if(sockfd == 0){ //Nur temrporaer um compile fhelrer zu verhindern
    printf("nothing");
  }
  return 0;
}
