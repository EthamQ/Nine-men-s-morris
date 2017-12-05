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
#include "shm_data.h"
#define BUF 256
#define GAMEKINDNAME "NMMORRIS"
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define BUF_SIZE 256
#define MES_LENGTH_SERVER 100
#define ATTEMPTS_INVALID 20

//initConnect uebernimmt die Aufgabe von main() zur Besserung Kapselung
int initConnect(){
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
              perror("Fehler bei Socket");
          continue;
          }

      if (connect(sockfd, p -> ai_addr,p -> ai_addrlen) < 0) {
          perror("fehler bei connect");
          printf("Socketzahl:%d\n",sockfd);
          close(sockfd);
          continue;

          }
          else{
          fprintf(stderr,"connected\n");
        }
          break; // if we get here, we must have connected successfully
      }

      if (p == NULL) {
      // looped off the end of the list with no connection
      fprintf(stderr, "failed to connect\n");
      //exit(2);
      return -1;
    }

  freeaddrinfo(servinfo); // all done with this structure // Brauche ich addrinfop fuer ahcfolgende funktionen ???

return sockfd;
}



int fork_thinker_connector(){
  pid_t pid;
int sockfd;
  switch(pid = fork()){
    case -1: perror("Fehler bei fork\n");
      return -1;
      break;
    case 0: printf("Kindprozess(Connector) mit der id %d und der Variable pid = %d. Mein Elternprozess ist: %d\n", getpid(), pid, getppid());
      //Connector

	//Verbindsaufbau zum Server
      if((sockfd = initConnect()) < 0){
        perror("Fehler bei initConnect");
      return -1;
	}
      else{
        printf("initConnect success\n");
      return -1;
	}

	//Prologphase
      if(performConnection(sockfd) < 0) {
          perror("Fehler bei performConnection");
      return -1;
	}
      else {
          printf("performConnection success");
      }
      
	exit(0);
      break;
    default: printf("Elternprozess(Thinker) mit der id %d und der Variable pid = %d. MeinElternprozess ist: %d\n", getpid(), pid, getppid());
      //Code for Thinker
      //Nicht in Meilenstein 2 implementiert
	
	//Elterprozess vererbt shared memory an Kindprozess, also attach hier im Elternprozess
	attachSHM();      


	wait(NULL);
      break;
  }

return 0;
}


int main(){
	//Shared memory erstellen
	int shmid;
	if((shmid = createSHM()) < 0){
	perror("Fehler bei Erstellung der shared memory");
	return -1;
	}
	else{
	printf("shared memory success");
	}

fork_thinker_connector();
return 0;
}
