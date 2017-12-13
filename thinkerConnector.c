#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <netdb.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdbool.h>
#include"performConnection.h"
#include "config.h"
#include "shm_data.h"
#include "drawfield.h"
#define BUF 256
#define GAMEKINDNAME "NMMORRIS"
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define BUF_SIZE 256
#define MES_LENGTH_SERVER 100
#define ATTEMPTS_INVALID 20
#define CONFIG_DEFAULT "client.conf"

//initConnect uebernimmt die Aufgabe von main() zur Besserung Kapselung
int initConnect(){
printf("\ninitConnect() gestartet.\n");
      int sockfd;
      int rv;

      struct addrinfo hints, *servinfo, *p;
      memset(&hints, 0, sizeof hints);
      hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
      hints.ai_socktype = SOCK_STREAM;

      if ((rv = getaddrinfo("sysprak.priv.lab.nm.ifi.lmu.de", "1357", &hints, &servinfo)) != 0) {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
      //exit(1);
      return -1;
      }

  // loop through all the results and connect to the first we can
      for(p = servinfo; p != NULL; p = p->ai_next) {
          if ((sockfd = socket(p->ai_family, p->ai_socktype,
              p->ai_protocol)) == -1) {
              //perror("Fehler bei Socket");
          continue;
          }

      if (connect(sockfd, p -> ai_addr,p -> ai_addrlen) < 0) {
          //perror("fehler bei connect");
          //printf("\nSocket file descriptor: %d\n",sockfd);
          close(sockfd);
          continue;

          }
          else{
          printf("\ninitConnect(): Connected\n");
          printf("Socket file descriptor: %d\n",sockfd);
	
        }
          break; // if we get here, we must have connected successfully
      }

      if (p == NULL) {
      // looped off the end of the list with no connection
      fprintf(stderr, "\ninitConnect(): Failed to connect\n");
      return -1;
    }

  freeaddrinfo(servinfo); // all done with this structure // Brauche ich addrinfop fuer ahcfolgende funktionen ???

printf("\ninitConnect() fertig ausgeführt.\n");
return sockfd;
}



int fork_thinker_connector(){
printf("\nStarte fork_thinker_connector\n");
  pid_t pid;
int sockfd;
  switch(pid = fork()){
    case -1: perror("\nfork_thinker_connector: Fehler bei fork\n");
      return -1;
      break;
    case 0: printf("fork_thinker_connector: Kindprozess(Connector) mit der id %d und der Variable pid = %d.\n Mein Elternprozess ist: %d\n\n", getpid(), pid, getppid());
      //Connector

	//Verbindsaufbau zum Server
      if((sockfd = initConnect()) < 0){
        perror("fork_thinker_connector: Fehler bei initConnect\n");
      return -1;
	}
      else{
        printf("fork_thinker_connector: initConnect success\n");
      return -1;
	}

	//Prologphase
      if(performConnection(sockfd) < 0) {
          perror("fork_thinker_connector: Fehler bei performConnection");
      return -1;
	}
      else {
          printf("fork_thinker_connector: performConnection success");
      }
      
	//Exit connector
	exit(0);
      break;
    default: printf("Elternprozess(Thinker) mit der id %d und der Variable pid = %d.\nMeinElternprozess ist: %d\n", getpid(), pid, getppid());
      //Code for Thinker
	//Elterprozess vererbt shared memory an Kindprozess, also attach hier im Elternprozess
	attachSHM();      
	
	//Warten bis Kindprozess beendet wurde
	wait(NULL);
      break;
  }

return 0;
}

char* game_id;
char* configname;
//Weißt die Kommandozeilenparameter Variablen zu
void init_cmd_args(char* gameID, char* config){
printf("Starte init_cmd_args\n\n");
printf("GameID: %s", gameID);
printf("config Pfad: %s\n\n", config);
game_id = gameID;
configname = config;
}

int main(int argc, char *argv[]){
//Kommandozeilenparameter
if(argc > 1){
if(argc > 2){
init_cmd_args(argv[1], argv[2]);
}
else{
init_cmd_args(argv[1], CONFIG_DEFAULT);
}
}

read_configfile();
drawField();
//Shared memory erstellen
	int shmid;
	if((shmid = createSHM()) < 0){
	perror("\nmain: Fehler bei Erstellung der shared memory\n");
	return -1;
	}
	else{
	printf("\nmain: shared memory success\n");
	}

//Thinker Connector
fork_thinker_connector();
return 0;
}
