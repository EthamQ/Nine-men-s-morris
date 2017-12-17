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
#include <fcntl.h>
#include <signal.h>
#include<unistd.h>
#include<string.h>
#include<stdbool.h>
#include"performConnection.h"
#include "shm_data.h"
#include "debugging.h"
#include "config_header.h"
#include "brain.h"
#include <sys/shm.h>

#define BUF 256
#define GAMEKINDNAME "NMMORRIS"
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define BUF_SIZE 256
#define MES_LENGTH_SERVER 100
#define ATTEMPTS_INVALID 20

#define CONFIG_DEFAULT "client.conf"

short gotSignal = 0;
short gameOver = 0;
char moveDest[5] = "Test";
int pipeFd[2];


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

//Spielzug an Connector schicken / in die Pipe schreiben
short sendMove(){
  char *pipeBuffer= think();
  printf("pipebuffer: %s \n", pipeBuffer);

  int gesendeteBytes = sizeof(pipeBuffer); //der return wert von write ist die anzahl der gesendeten bytes, falls das != der zu sendenden bytes PANIK !

  if( (write (pipeFd[1], pipeBuffer, gesendeteBytes) ) != gesendeteBytes) {
       perror("Fehler beim schreiben des Spielzugs in das pipe, BRAIN");
       return -1;
    }
  printf("Spielzug in die Pipe geschrieben, BRAIN \n");
  //sleep(0.5);
  return 0;
}

void signalHandlerThinker(int signalNum){
  if(signalNum==SIGUSR1){


	sendMove();
}
}

int fork_thinker_connector(){

	  printf("\nStarte fork_thinker_connector\n");

	  //Fork Variablen
	  pid_t pid;
	  int sockfd;

	  //Pipe Variablen
	  char movePipe[5];

	  //Erstellung der Pipe, muss vor Fork geschehen
	  if (pipe (pipeFd) < 0) {
	      perror ("Fehler bei Erstellung der Pipe");
	      return -1;
	   }

	int shmid;
	if((shmid = createSHM()) < 0){
	perror("Fehler bei Erstellung der shared memory");
	return -1;
	}
	else{
	printf("shared memory success");
	}





 

  switch(pid = fork()){
    case -1: perror("Fehler bei fork\n");
      return -1;
      break;
    case 0: printf("Kindprozess(Connector) mit der id %d und der Variable pid = %d. Mein Elternprozess ist: %d\n", getpid(), pid, getppid());
      //Connector

			//Schreibseite der Pipe schliessen
      close(pipeFd[1]);

			//Verbindsaufbau zum Server
      if((sockfd = initConnect()) < 0){
        perror("Fehler bei initConnect");
      return -1;
			}
      else{
        printf("initConnect success\n");
			}

			//Prologphase
      if(performConnection(sockfd) < 0) {
      	  printf("%i",sockfd);
          perror("Fehler bei performConnection");
      return -1;
			}
      else {
          printf("performConnection success");
      }
			//Signal an Thinker senden
			if(kill(getppid(),SIGUSR1)<0){
					perror("Fehler bei senden des Signals an den Thinker, CONNECTOR");
			}
			printf("Signal an Thinker gesendet, CONNECTOR \n");

			//Aus der Pipe den Spielzug lesen

			if(((read (pipeFd[0], movePipe, 5)) == 5)){//&&(movePipe != "")){
				printf("Spielzug aus Pipe gelesen: %s \n", movePipe);
			}
			else{
				perror("Spielzug konnte nicht aus der Pipe gelesen werden");
			}

			exit(0);
      break;
    default: printf("Elternprozess(Thinker) mit der id %d und der Variable pid = %d. MeinElternprozess ist: %d\n", getpid(), pid, getppid());
      //Code for Thinker
      //Nicht in Meilenstein 2 implementiert


			//Leseseite der Pipe schliessen
			close (pipeFd[0]);

			//Signalhandler fuer Connector Signal definieren
			//http://pubs.opengroup.org/onlinepubs/009695399/functions/sigaction.html muss code nochmal pruefen
			struct sigaction sa;
			sa.sa_handler = signalHandlerThinker;
			sigemptyset(&sa.sa_mask);
			sa.sa_flags = SA_RESTART;
			printf("Signalhandler definiert THINKER \n");

			if(sigaction(SIGUSR1, NULL , NULL ) > 0){///!!!!!!
				printf("sigaction sucess THINKER");
			}
			else{
				perror("sigaction fehler ???");
			}

	
	//Elterprozess vererbt shared memory an Kindprozess, also attach hier im Elternprozess
shmat(shmid, NULL, 0);


			wait(NULL);
      break;
  }

return 0;
}


int main(){
	drawField();
	read_configfile(CONFIG_DEFAULT);

	fork_thinker_connector();
return 0;
}
