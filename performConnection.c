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
static char versionPRC []= "VERSION 2.0\n";
//static char game_idPRC []= "ID 02pobsvmluimp\n";
//static char game_idPRC []= "ID 2uiyd4c9217om\n";
//static char game_idPRC []= "ID 2uiyd4c9217om\n";
static char game_idPRC []= "ID 052j3mkakfco2\n";
static char numberOfPlayersPRC []= "PLAYER\n";
static char thinkingPRC []= "THINKING\n";

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
      perror("Invalid socket file descriptor");
      close(sockfd);
      return ERROR;
    }
    else{
      printf("Good to go?!\n");
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
        perror("Fehler bei Gameserver Version\n");
        return ERROR;
      }
    }
    char *ptr = dataPRS;
    int i = 0;
    int a,b = 0;
    int counter = 0;
    for(i;i<strlen(ptr);i++){
	int zahl = ptr[i] - '0';
	if(zahl >= 0 && zahl <= 9) {
           if(counter == 0) {
	       	a = zahl;
		counter++;
	   }
	   else {
		   b = zahl;
	   	   break;
	   }
	}
    }
           
    printf("Spiel wird auf Version v%i.%i gespielt\n",a,b);
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
    printf("Sende Clientversion:%s\n",versionPRC);
    testifvalid = -1;
	  attempts = 0;

    //S: Game-ID request
    while(testifvalid < 0){
        testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
	      attempts++;
        if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
            perror("Fehler bei Game-ID request\n");
            return -1;
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
            return -1;
        }
    }

    
    printf("Spiel hat Game-%s",game_idPRC);
    testifvalid = -1;
	  attempts = 0;

    //S: <<Gamekind-Name>>
    while(testifvalid < 0){
        testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
        if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
            perror("Fehler bei Gamekind-Name");
            return -1;
        }
    }
    printf("gespielt wird im folgenden Modus:%s",dataPRS);
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
      perror("Fehler bei ENDPLAYERS-Nachricht");
      return -1;
      }
    }
    testifvalid = -1;
	  attempts = 0;
	  read_piecelist(shm_pointer,dataPRS);

    //C: THINKING
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
    /*dataPRS leeren, damit OKTHINK think nicht den anderen inhalt von dataPRS ueberschreibt
      TODO Spaeter sollten wir vllt den Inahlt von dataPRS in eine anderes charray schreiben
      und das dann nach "C: THINKING" ins shared memeory schreiben, wobei das vllt gar nicht noetig ist,
      weil die intialpositionen und PIECELIST eh immer gleich ist
    */
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
 
 return ERROR;
}
