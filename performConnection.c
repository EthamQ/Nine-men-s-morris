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
#include "constants.h"
#include "shm_data.h"

static char dataPRS[MES_LENGTH_SERVER];
static char versionPRC[BUFFER_SIZE];
static char game_idPRC[BUFFER_SIZE];
static char numberOfPlayersPRC[BUFFER_SIZE];

//ARGS: server message, if it begins with "+" return true
static bool serverResponseValid(const char r[]){
    if(strncmp(r, "+",1) == 0){
        return true;
    }
      return false;
}

int performConnection(int sockfd, struct SHM_data* shm_pointer){
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
    strcpy(versionPRC,confiConst.gameVersion);
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
    strcpy(game_idPRC,confiConst.gameID);
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
    strcpy(numberOfPlayersPRC,confiConst.playerNumber);
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
	  read_piecelist(shm_pointer,dataPRS);

	  //---------Entscheiden Move oder wait-----------------
    //C: THINKING
	if(strstr(dataPRS,"+ MOVE ")){
		//send THINKING
		printf("\nperformConnection(): received +MOVE from the server\n");
		if(write(sockfd, THINKING_MSG, (int)strlen(THINKING_MSG)) <0){
			perror("Fehler beim senden von THINKING");
		}
		printf("C: %s", THINKING_MSG);

	memset(&dataPRS[0], 0, sizeof(dataPRS));
    printf("\nC: THINKING\n");

  //S:+ OKTHINK
  while(testifvalid < 0){
    testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
    //printf("\nOKthink ???\n\n");
    printf("S: %s\n",dataPRS);
    if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
      perror("Invalid server response5");
      return -1;
    }
  }

  //Auf thinking darf nur okthink folgen, sonst ist vorher etwas schiefgelaufen
  if(strstr(dataPRS,"+ OKTHINK")){
     //printf("\nperform Connection tells maintainConnection.c that the Server sent +OKTHINK");
	 //Aufruf von Spielzug PLAY ...
     return OKTHINK;
 }

	}

 return ERROR;
}
