#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/shm.h>

#include "performConnection.h"
#include "shm_data.h"
#include "debugging.h"
#include "config_header.h"
#include "brain.h"
#include "maintainConnection.h"
#include "constants.h"

short gotSignal = 0;
short gameOver = 0;
char moveDest[5] = "Test";
int pipeFd[2];
int *shmptr_global;
int shmid_g;

//Connect to the server and return the socket file descriptor
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

//Normalen Spielzug an Connector schicken / in die Pipe schreiben
short sendMove(){
  char *pipeBuffer= malloc(sizeof(char)*(256));
  //pipeBuffer=think();
  //printf("SPeicheradresse von shm global: %p",shmptr_global);
  struct SHM_data* shm_pointer = shmat(shmid_g, NULL, 0);
  pipeBuffer=think_new(shm_pointer);
  printf(" Thinker berechneter Zug: %s\n ",pipeBuffer);
  printf("pipebuffer: %s \n", pipeBuffer);




  if((write(pipeFd[1], pipeBuffer, sizeof(pipeBuffer)))<=0){

       perror("Fehler beim schreiben des Spielzugs in die pipe, BRAIN");
       return -1;
    }
  printf("Spielzug in die Pipe geschrieben, THINKER \n");
  free(pipeBuffer);
  return 0;
}

//Capture Spielzug an Connector schicken / in die Pipe schreiben
short sendCaptureMove(){
  char *pipeBuffer= malloc(sizeof(char)*(256));
  //ppipeBuffer=think_new(shm_pointer);
  printf(" Thinker berechneter capture Zug: %s\n ",pipeBuffer);
  printf("pipebuffer: %s \n", pipeBuffer);
  if((write(pipeFd[1], pipeBuffer, sizeof(pipeBuffer)))<=0){
       perror("Fehler beim schreiben des CAPTURE Spielzugs in die pipe, BRAIN");
       return -1;
    }
  printf("CAPTURE Spielzug in die Pipe geschrieben, THINKER \n");
  free(pipeBuffer);
  return 0;
}

static void signalHandlerThinker(int signalNum){
  printf("Signalhandler go?\n");
  if(signalNum==SIGUSR1){
    printf("Signal SIGUSR1 angekommen\n");
	sendMove();
  }
  if(signalNum==SIGUSR2){
    printf("Signal SIGUSR2 angekommen\n");
	sendCaptureMove();
  }
}


	//task: entweder MOVE für SIGUSR1 oder CAPTURE für SIGUSR2
	//Sendet entsprechendes Signal an den Thinker und liest dann den Spielzug aus der pipe
	//und sendet ihn an conPlay, der die passende Nachricht an den Server sendet
	void send_signal(int sockfd, int task, char* movePipe){
	if(task == MOVE){
			//SIGUSR1 Signal an den thinker
			if(kill(getppid(),SIGUSR1)<0){
			perror("Fehler bei senden von SIGUSR1 an den Thinker, CONNECTOR");
			}
			printf("Signal an Thinker gesendet,erster spielzug, CONNECTOR \n");
			//sleep(1); //TODO ist sleep hier notwendig ?
			//Aus der Pipe den Spielzug lesen
			if((read (pipeFd[0], movePipe, sizeof(movePipe))) >0){
			printf("Spielzug aus Pipe gelesen: %s \n", movePipe);
			}
			else{
			perror("Spielzug konnte nicht aus der Pipe gelesen werden");
			}
			//Spielzug an den Server senden
			if(send_move_to_server(sockfd, movePipe) == ERROR){
			perror("conplay failure, THINKCON");
			}
		}

	if(task == CAPTURE){
			//SIGUSR2 Signal an den thinker
			if(kill(getppid(),SIGUSR2)<0){
			perror("Fehler bei senden von SIGUSR2 an den Thinker, CONNECTOR");
			}
			printf("Signal an Thinker gesendet,erster spielzug, CONNECTOR \n");
			//sleep(1); //TODO ist sleep hier notwendig ?
			//Aus der Pipe den Spielzug lesen
			if((read (pipeFd[0], movePipe, sizeof(movePipe))) >0){
			printf("Spielzug aus Pipe gelesen: %s \n", movePipe);
			}
			else{
			perror("Spielzug konnte nicht aus der Pipe gelesen werden");
			}
			//Spielzug an den Server senden
			if(send_move_to_server(sockfd, movePipe) == ERROR){
			perror("conplay failure, THINKCON");
			}
		}

	}


int fork_thinker_connector(){
  printf("\nStarte fork_thinker_connector\n");

  //Fork Variablen
  pid_t pid;
  int sockfd;

  //Pipe BUFFER
  char *movePipe=malloc(sizeof(char)*(PIPE_BUF));

  //Erstellung der Pipe, muss vor Fork geschehen
  if (pipe(pipeFd) < 0) {
      perror ("fork_thinker_connector(): Fehler bei Erstellung der Pipe");
      return -1;
   }
   else{
     printf("fork_thinker_connector(): Created pipe successfully");
   }

	int shmid;
	if((shmid = createSHM()) < 0){
	   perror("fork_thinker_connector(): Fehler bei Erstellung der shared memory");
	   return -1;
	}
	else{
	   printf("fork_thinker_connector(): shared memory success");
	   shmid_g = shmid;
	}
	//*shmptr_global = shmat(shmid, NULL, 0);

	//FORK
  switch(pid = fork()){
    case -1: perror("fork_thinker_connector(): Fehler bei fork\n");
      return -1;
      break;
	  //CONNECTOR
    case 0: printf("Kindprozess(Connector) mit der id %d und der Variable pid = %d. Mein Elternprozess ist: %d\n", getpid(), pid, getppid());
		//temporary: short endCon = 0;

		//Schreibseite der Pipe schliessen
		close(pipeFd[1]);

		//SERVERVERBINDUNG
      if((sockfd = initConnect()) < 0){
        perror("\nfork_thinker_connector(): Fehler bei initConnect");
        return -1;
			}
      else{
        printf("\nfork_thinker_connector(): initConnect success\n");
			}


	//shm test
	  struct SHM_data* shm_pointer = shmat(shmid, NULL, 0);
	  //writeSHM(a, "HELLO", SPIELNAME);
	  //readSHM(a);

  	//PROLOG
  	//int first_command = (int)performConnection(sockfd);
    if(performConnection(sockfd, shm_pointer) < 0) {
        perror("\nfork_thinker_connector(): Fehler bei performConnection");
        return -1;
  	}
    else {
        printf("\nfork_thinker_connector(): performConnection success \n");
    }
    //Perfcon endet mit einem THINKING, d.h. unmittelbar darauf muss ein PLAY folgen
    //#############################
    //HIER PLAY WICHTIG TODO !!!!!!
    //#############################
    //Signal an Thinker senden, erster spielzug des spiels
     send_signal(sockfd, MOVE, movePipe);



	//int n = 200;
	  while(1){
		 // n--;
		  switch(maintainConnection(sockfd, shm_pointer)){
			case MOVE:
				//sends SIGUSR1
				send_signal(sockfd, MOVE, movePipe);
				break;

			case CAPTURE:
				//sends SIGUSR2
				send_signal(sockfd, CAPTURE, movePipe);
				break;

			case WAIT:
				write(sockfd, "OKWAIT", sizeof(char)*10);
				printf("C: OKWAIT");
				break;

			case MOVEOK: break;
			case GAMEOVER: printf("S: GAMEOVER");break;
			case ERROR: printf("CASE ERROR"); sleep(1); break;
		  }

	  }

	  /*
	  maintainConnection(sockfd);
	  char *serverResponse=malloc(sizeof(char)*MES_LENGTH_SERVER);
	  read(sockfd, serverResponse, sizeof(char)*MES_LENGTH_SERVER);
	  printf("S: %s", serverResponse);


	  write(sockfd, "OKWAIT", sizeof(char)*MES_LENGTH_SERVER);
	  printf("C: OKWAIT");

	  read(sockfd, serverResponse, sizeof(char)*MES_LENGTH_SERVER);
	  printf("S: %s", serverResponse);
	  */






    //Jetzt koennen wir in den normalen SPielverlauf uebergehen
      /*while(1){
        switch(maintainConnection(sockfd)){
          case WAIT:
            printf("conWait Aufruf, THINKCON");
            if(conWAIT(sockfd)<0){
              perror("conWAIT, CONNECTOR");
            }
            break;
          case GAMEOVER:
            printf("conGAMEOVER Aufruf, THINKCON");
            if(conGAMEOVER(sockfd)<0){
              perror("conGAMEOVER failure, CONNECTOR");
            }
            break;
          case MOVE:
            printf("conMOVE Aufruf, THINKCON");
            if(conMOVE(sockfd)<0){
              perror("conGAMEOVER failure, CONNECTOR");
            }
            //Signal an Thinker senden
            if(kill(getppid(),SIGUSR1)<0){
                perror("Fehler bei senden des Signals an den Thinker, CONNECTOR");
            }
            printf("Signal an Thinker gesendet, CONNECTOR \n");
            sleep(1); //TODO ist sleep hier notwendig ?
            //Aus der Pipe den Spielzug lesen
            if((read (pipeFd[0], movePipe, sizeof(movePipe))) >0){
              printf("Spielzug aus Pipe gelesen: %s \n", movePipe);
            }
            else{
              perror("Spielzug konnte nicht aus der Pipe gelesen werden");
            }
            break;
          default:
            perror("\nSwitch failure CONNECTOR\n");
			//printf("\nConnector: In switch case wurde folgender Wert gesucht: %i\n", test);
            endCon = -1;
            break;
        }
        if(endCon == -1){
          break;
        }

      }
      //printf("Movepipe, aus compilergruenden: %s \n", movePipe); //TODO entfernen
			exit(0);
      break;
	  */
	  //>>=======THINKER=======<<
    default: printf("Elternprozess(Thinker) mit der id %d und der Variable pid = %d. MeinElternprozess ist: %d\n", getpid(), pid, getppid());

			//Leseseite der Pipe schliessen
			close (pipeFd[0]);

			//Signalhandler fuer Connector Signal definieren
			//http://pubs.opengroup.org/onlinepubs/009695399/functions/sigaction.html muss code nochmal pruefen
			struct sigaction sa;
			sa.sa_handler = signalHandlerThinker;
			sigemptyset(&sa.sa_mask);
			sa.sa_flags = SA_RESTART;
			printf("Signalhandler definiert THINKER \n");

			if(sigaction(SIGUSR1, &sa , NULL ) == 0){ //Bei success 0, sonst -1
				printf("sigaction sucess THINKER\n");
			}
			else{
				perror("sigaction groesser Null, Fehler ???");
			}
			if(sigaction(SIGUSR2, &sa , NULL ) == 0){ //Bei success 0, sonst -1
				printf("sigaction sucess THINKER\n");
			}
			else{
				perror("sigaction groesser Null, Fehler ???");
			}

	    //Elterprozess vererbt shared memory an Kindprozess, also attach hier im Elternprozess
      shmat(shmid, NULL, 0);
      printf("THINKER: jetzt beginnt das warten\n");

			wait(NULL);
      break;
  }
  free(movePipe);
  return 0;
}

int main(int argc, char *argv[]){
  short paras;
  char* parGameId;
  char* parPlayerNumber;
  char parConfigFileLocation[256];
  strcpy(parConfigFileLocation,"");
  //Auslesen der Parameter
  while( (paras = getopt(argc, argv, "g:p:c:")) != -1){
    switch(paras){
      case 'g':
                parGameId = optarg;
                if((strstr(parGameId, "-")) != NULL){
                  parGameId = " ";
                }
                printf("\ngameid: %s",optarg);
                break;
      case 'p':
                parPlayerNumber = optarg;
                printf("\nplayernumber: %s \n", parPlayerNumber);
                break;
      case 'c':
                printf("\nparameter c: \"%s\"",optarg);
                strcpy(parConfigFileLocation,optarg);
                printf("\nparameter c: %s",optarg);
                break;
      default:
                perror("Fehler: ungueltiger Parameter, THINKCON");
    }
  }
  if( (strcmp(parPlayerNumber,"1") == 0) || (strcmp(parPlayerNumber,"2") == 0) ){
  }
  else{
    parPlayerNumber = " ";
  }
  if( (strcmp(parConfigFileLocation," ") == 0) || (strcmp(parConfigFileLocation,"") == 0) ){
    strcpy(parConfigFileLocation," ");
  }

  if(read_configfile(parGameId, parPlayerNumber, parConfigFileLocation) == -1){
    perror("Terrible Failure in readConfig.c , THINKCON");
    return -1; //TODO vllt einfach iwleche standardwerte assignen, statt abzustuerzen ???
  }
return 0;
}
