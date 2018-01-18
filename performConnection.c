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
//included Headerfiles
#include "constants.h"
#include "shm_data.h"

static char dataPRS[MES_LENGTH_SERVER];

//ARGS: server message, if it begins with "+" return true
static bool serverResponseValid(const char r[]){
    if(strncmp(r, "+",1) == 0){
        return true;
    }
      return false;
}




int performConnection(int sockfd, struct SHM_data* shm_pointer){
  char versionPRC []= confiConst.gameVersion;
  char game_idPRC []= confiConst.gameID;
  char numberOfPlayersPRC []= confiConst.playerNumber;
    if(sockfd < 0){
      perror("Fehler bei sockfd");
      close(sockfd);
      return ERROR;
    }
    else{
      printf("Beginne PerformConnection");
    }
    //if testifvalid is negative then there was an error, repeat write() or read() if error
    ssize_t testifvalid = -1;
    //Number of invalid attempts you're allowed to have, amount -> see #define ATTEMPTS_INVALID, counts attempts up to this number
    int attempts = 0;

    //S: <<Gameserver Version>>
    while(testifvalid < 0){
      testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
      attempts++;
      if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
        perror("Fehler bei Server Version");
        return ERROR;
      }
    }
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
    printf("Client ist auf Version: %s\n",versionPRC);
    testifvalid = -1;
	  attempts = 0;

    //S: Game-ID request
    while(testifvalid < 0){
        testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
	      attempts++;
        if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
            perror("Fehler bei Game-ID");
            return ERROR;
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
            return ERROR;
        }
    }
    printf("Spiel mit der %s wurde erstellt\n",game_idPRC);
    testifvalid = -1;
    attempts = 0;

    //S: <<Gamekind-Name>>
    while(testifvalid < 0){
        testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
        if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
            perror("Fehler bei Gamekind-Name\n");
            return ERROR;
        }
    }
    printf("Gespielt wird im Modus %s\n",dataPRS);
    testifvalid = -1;
    attempts = 0;

    //C: [[GewÅ¸nschte Mitspielernummer]]
    while(testifvalid < 0){
        testifvalid = write(sockfd, numberOfPlayersPRC, (int)strlen(numberOfPlayersPRC));
        attempts++;
        if(attempts >= ATTEMPTS_INVALID){
            return ERROR;
        }
    }
    printf("Anzahl der teilnehmenden Spieler: &c",numberOfPlayersPRC);
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
      perror("Fehler bei Endplayers bzw Endpiecelist");
      return ERROR;
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
     printf("\nperform Connection tells maintainConnection.c that the Server sent +OKTHINK");
	 //Aufruf von Spielzug PLAY ...
     return OKTHINK;
 }

	}

 return ERROR;
}
