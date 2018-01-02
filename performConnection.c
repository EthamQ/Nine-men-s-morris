#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <netdb.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>

#include"performConnection.h"
#include "constants.h"

static char dataPRS[MES_LENGTH_SERVER];
static char versionPRC []= "VERSION 2.0\n";
static char game_idPRC []= "ID 02pobsvmluimp\n";
static char numberOfPlayersPRC []= "PLAYER\n";
static char thinkingPRC []= "THINKING\n";

//ARGS: server message, if it begins with "+" return true
static bool serverResponseValid(const char r[]){
    if(strncmp(r, "+",1) == 0){
        return true;
    }
      return false;
}

/*
reads and returns the first server command
Information important for int maintainConnectionFIRST(int sockfd, int firstServerCommand) in maintainConnection.c
*/
int performConnection(int sockfd){
    //char *serverPiecelist=malloc(sizeof(char)*1048); //TODO Free
    if(sockfd < 0){
      printf("%dtest",sockfd);
      perror("Invalid socket file descriptor");
      close(sockfd);
      return ERROR;
    }
    else{
      printf("Good to go?!");
    }
    //if testifvalid is negative then there was an error, repeat write() or read() if error
    ssize_t testifvalid = -1;
    //Number of invalid attempts you're allowed to have, amount -> see #define ATTEMPTS_INVALID, counts attempts up to this number
    int attempts = 0;

    //TODO: if Server doesn't respond with "+" -> error handling
    //S: <<Gameserver Version>>
    while(testifvalid < 0){
      testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
      attempts++;
      if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
        perror("Invalid server response2");
        printf("%sfehlertest",dataPRS);
        return ERROR;
      }
    }
    printf("%s\n",dataPRS);
    testifvalid = -1;
    attempts = 0;

    //C: <<Client Version>>
    while(testifvalid < 0){
        testifvalid = write(sockfd, versionPRC, (int)strlen(versionPRC));
        attempts++;
        if(attempts >= ATTEMPTS_INVALID){
            return ERROR;
        }
    }
    printf("%s\n",versionPRC);
    testifvalid = -1;
	  attempts = 0;

    //S: Game-ID request
    while(testifvalid < 0){
        testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
	      attempts++;
        if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
            perror("Invalid server response3");
            printf("%s\n",dataPRS);
            return -1;
        }
    }
    printf("%s\n",dataPRS);
    testifvalid = -1;
	  attempts = 0;

    //C: <<Game-ID>>
    while(testifvalid < 0){
        testifvalid = write(sockfd, game_idPRC, (int)strlen(game_idPRC));
        attempts++;
        if(attempts >= ATTEMPTS_INVALID){
            return -1;
        }
    }
    printf("%s\n",game_idPRC);
    testifvalid = -1;
	  attempts = 0;

    //S: <<Gamekind-Name>>
    while(testifvalid < 0){
        testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
        if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
            perror("Invalid server response4");
            return -1;
        }
    }
    printf("%s\n",dataPRS);
    testifvalid = -1;
	  attempts = 0;

    //C: [[GewÅ¸nschte Mitspielernummer]]
    while(testifvalid < 0){
        testifvalid = write(sockfd, numberOfPlayersPRC, (int)strlen(numberOfPlayersPRC));
        attempts++;
        if(attempts >= ATTEMPTS_INVALID){
            return -1;
        }
    }
    testifvalid = -1;
	  attempts = 0;

    //S: <<Mitspielernummer>> <<Mitspielername>>
    //S: <<Mitspieleranzahl>>
    //S: <<Mitspielernummer>> <<Mitspielername>> <<Bereit>>
    //S: ENDPLAYERS
    //S:+MOVE 3000
    //S:CAPTURE 0
    //S:<<PIECELIST>>
    //S:+ENDPIECELIST

    while(testifvalid < 0){
      testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
      printf("%s\n",dataPRS);
      if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
      perror("Invalid server response5");
      return -1;
      }
    }
    testifvalid = -1;
	  attempts = 0;


    //C: THINKING
    printf("anfang thinken senden, PERFCON \n");
    while(testifvalid < 0){
        testifvalid = write(sockfd, thinkingPRC, (int)strlen(thinkingPRC));
        attempts++;
        if(attempts >= ATTEMPTS_INVALID){
            printf("Fehler beim senden von THINKING, PERFCON");
            return -1;
        }
    }
    testifvalid = -1;
    attempts = 0;
    printf("thinking sollte gesendet sein, PERFCON");

  //S:+ OKTHINK
  while(testifvalid < 0){
    testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
    printf("\nOKthink ???\n\n")
    printf("%s\n",dataPRS);
    if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
      perror("Invalid server response5");
      return -1;
    }
  }
  testifvalid = -1;
  attempts = 0;

    if(strstr(dataPRS,"+ OKTHINK")){
       printf("perform Connection tells maintainConnection.c that the Server sent +MOVE");
       return OKTHINK;
   }

	 if(strstr(dataPRS,"+ MOVE")){
      printf("perform Connection tells maintainConnection.c that the Server sent +MOVE");
      return MOVE;
	 }

	  if(strstr(dataPRS,"+ WAIT")){
      printf("perform Connection tells maintainConnection.c that the Server sent +WAIT");
      return WAIT;
	 }

	  if(strstr(dataPRS,"+ GAMEOVER")){
      printf("perform Connection tells maintainConnection.c that the Server sent +GAMEOVER");
      return GAMEOVER;
	 }

	 return ERROR;
}
