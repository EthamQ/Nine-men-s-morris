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
  char versionPRC[BUFFER_SIZE];
  char game_idPRC[BUFFER_SIZE];
  char numberOfPlayersPRC[BUFFER_SIZE];
  char mode[BUFFER_SIZE];
  strcpy(versionPRC,confiConst.gameVersion);
  strcpy(game_idPRC,confiConst.gameID);
  strcpy(numberOfPlayersPRC,confiConst.playerNumber);
  strcpy(mode,confiConst.gameKindName);

    if(sockfd < 0){
      perror("Fehler bei sockfd\n");
      close(sockfd);
      return ERROR;
    }
    else{
      //printf("\nBeginne PerformConnection\n");
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
        perror("Fehler bei Server Version\n");
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
    printf("\nClient ist auf Version: %s\n",versionPRC);
    testifvalid = -1;
	  attempts = 0;

    //S: Game-ID request
    while(testifvalid < 0){
        testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
	      attempts++;
        if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
            perror("Fehler bei Game-ID\n");
            return ERROR;
        }
    }
    printf("Server benötigt Game-ID\n");
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
    printf("Spiel erstellt mit %s\n",game_idPRC);
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
    printf("Gespielt wird %s\n",mode);
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
    printf("Eingegebene Mitspielernummer: %s\n",numberOfPlayersPRC);

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
      //printf("%s\n",dataPRS);
      if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
      perror("Fehler bei Endplayers bzw Endpiecelist\n");
      return ERROR;
      }
    }
    testifvalid = -1;
	  attempts = 0;
	  
	  if(shm_pointer->opponent.flag_registriert) {
		printf("\nGegner ist bereit\n");
	  }
	  else {
		printf("\nGegner ist noch nicht bereit\n");	
		printf("servermessage:%s",dataPRS);
	  }
	  read_piecelist(shm_pointer,dataPRS);

	  if(shm_pointer->client.spielernummer ==0){
	  printf("Client ist Spieler 1 und hat den Namen %s\n",shm_pointer->client.spielername);
	  printf("Der Gegner ist Spieler 2 und hat den Namen %s\n",shm_pointer-> opponent.spielername);
	  }
	  else{
	  printf("Client ist Spieler 2 und hat den Namen %s\n",shm_pointer->client.spielername);
	  printf("Der Gegner ist Spieler 1 und hat den Namen %s\n",shm_pointer-> opponent.spielername);
	  }
	  
	//---------Entscheiden Move oder wait-----------------
  //C: THINKING
	if(strstr(dataPRS,"+ MOVE ")){
		//send THINKING
		//printf("\nperformConnection(): received +MOVE from the server\n");
		if(write(sockfd, THINKING_MSG, (int)strlen(THINKING_MSG)) <0){
			perror("Fehler beim senden von THINKING\n");
		}
		//printf("C: %s", THINKING_MSG);

	 memset(&dataPRS[0], 0, sizeof(dataPRS));
    //printf("sende THINKING an den Server\n");

  //S:+ OKTHINK
  while(testifvalid < 0){
    testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
    //printf("\nOKthink ???\n\n");
    //printf("S: %s\n",dataPRS);
    if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
      perror("Fehler bei OKTHINK\n");
      return -1;
    }
  }

  //Auf thinking darf nur okthink folgen, sonst ist vorher etwas schiefgelaufen
  if(strstr(dataPRS,"+ OKTHINK")){
     //printf("perform Connection tells maintainConnection.c that the Server sent +OKTHINK\n");
	 //Aufruf von Spielzug PLAY ...
     return OKTHINK;
  }
 }

 return ERROR;
}
