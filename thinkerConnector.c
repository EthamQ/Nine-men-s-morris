#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
<<<<<<< HEAD
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h> //Fuer Prozesse
#include <sys/socket.h> //Fuer initConnect
#include <netinet/in.h>  //Fuer initConnect
#include <netdb.h> //Fuer initConnect
#include <arpa/inet.h> //Fuer initConnect
#include <fcntl.h> //Fuer Pipe
#include <signal.h> //Fuer Signal Connector->Thinker

#include "performConnection.h"
//#include "shm_data.h"
#include "brain.h"

=======
#include <sys/wait.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <netdb.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdbool.h>
#include "performConnection.h"
#include "config_header.h"
#include "shm_data.h"
#include "drawfield.h"
>>>>>>> 9878ab0d973ba94636da88daa0ec2461747de510
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
<<<<<<< HEAD
      fprintf(stderr, "failed to connect\n");
=======
      fprintf(stderr, "\ninitConnect(): Failed to connect\n");
>>>>>>> 9878ab0d973ba94636da88daa0ec2461747de510
      return -1;
    }

  freeaddrinfo(servinfo); // all done with this structure // Brauche ich addrinfop fuer ahcfolgende funktionen ???
  return sockfd;
}

<<<<<<< HEAD
void signalHandlerThinker(int signalNum){
  //TODO alles
  printf("Signalnummer: %i\n", signalNum);
  if(signalNum == SIGUSR1){
    if(gotSignal != 1){
      // ?????
      gotSignal = 1;
    }
    else{
      perror("Signalhandler Fehler, neues Signal waehrend altes signal noch verarbeitet wird");
    }
  }
=======
printf("\ninitConnect() fertig ausgeführt.\n");
return sockfd;
>>>>>>> 9878ab0d973ba94636da88daa0ec2461747de510
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

int fork_thinker_connector(){
<<<<<<< HEAD
  //Fork Variablen
=======
printf("\nStarte fork_thinker_connector\n");
>>>>>>> 9878ab0d973ba94636da88daa0ec2461747de510
  pid_t pid;
  int sockfd;

  //Pipe Variablen
  char movePipe[5];

  //Erstellung der Pipe, muss vor Fork geschehen
  if (pipe (pipeFd) < 0) {
      perror ("Fehler bei Erstellung der Pipe");
      return -1;
   }

  //Forken
  switch(pid = fork()){
    case -1: perror("\nfork_thinker_connector: Fehler bei fork\n");
      return -1;
      break;
    case 0: printf("fork_thinker_connector: Kindprozess(Connector) mit der id %d und der Variable pid = %d.\n Mein Elternprozess ist: %d\n\n", getpid(), pid, getppid());
      //Connector

      //Schreibseite der Pipe schliessen
      close(pipeFd[1]);

      //Verbindsaufbau zum Server
      if((sockfd = initConnect()) < 0){
<<<<<<< HEAD
        perror("Fehler bei initConnect");
        return -1;
    	}
      else{
        printf("initConnect success\n");
        //return -1;
      }
=======
        perror("fork_thinker_connector: Fehler bei initConnect\n");
      return -1;
	}
      else{
        printf("fork_thinker_connector: initConnect success\n");
      return -1;
	}
>>>>>>> 9878ab0d973ba94636da88daa0ec2461747de510

      printf("Beginne mit performConnection \n");
    	//Prologphase
      if(performConnection(sockfd) < 0) {
<<<<<<< HEAD
          perror("Fehler bei performConnection");
          return -1;
    	  }
      else {
          printf("performConnection success \n");
      }
      //Endlosschleife damit Prozess nicht einschlaeft oder sonstwas
      while(gameOver != 1){
        printf("Ich bin eine Endlosschleife :D  \n");
        //TODO Prologphase Teil 2


        //TODO Auf Negative Serverantwort oder Spielzug warten

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
        //}
    break;
    default: printf("Elternprozess(Thinker) mit der id %d und der Variable pid = %d. MeinElternprozess ist: %d \n", getpid(), pid, getppid());
      //Thinker

      //Elterprozess vererbt shared memory an Kindprozess, also attach hier im Elternprozess
      //attachSHM();

      //Leseseite der Pipe schliessen
      close (pipeFd[0]);

      //Signalhandler fuer Connector Signal definieren
      //http://pubs.opengroup.org/onlinepubs/009695399/functions/sigaction.html muss code nochmal pruefen
      struct sigaction sa;
      sa.sa_handler = signalHandlerThinker;
      sigemptyset(&sa.sa_mask);
      sa.sa_flags = SA_RESTART;
      printf("Sigalhandler definiert \n");

      //TODO While schleife ????
      while(gameOver != 1){
        if(gotSignal == 1){
          think();
          printf("Ich bin eine Endlosschleife, THINKER :D \n");
          gotSignal = 0; //gotSignal zuruecksetzen um auf neues Signal zu warten
        }
      }

    	wait(NULL); //AUf Child Warten
          break;
      }
=======
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
>>>>>>> 9878ab0d973ba94636da88daa0ec2461747de510
  }

return 0;
}

<<<<<<< HEAD
int main(){
  /*
	//Shared memory erstellen
=======
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
>>>>>>> 9878ab0d973ba94636da88daa0ec2461747de510
	int shmid;
	if((shmid = createSHM()) < 0){
	perror("\nmain: Fehler bei Erstellung der shared memory\n");
	return -1;
	}
	else{
	printf("\nmain: shared memory success\n");
	}
<<<<<<< HEAD
*/
  //Aufteilung in 2 Prozese
  fork_thinker_connector();
=======

//Thinker Connector
fork_thinker_connector();
>>>>>>> 9878ab0d973ba94636da88daa0ec2461747de510
return 0;
}
