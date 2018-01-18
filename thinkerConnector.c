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
int shmid;

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
          //printf("Socketzahl:%d\n",sockfd);
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
  //pipeBuffer=think();
  //printf("SPeicheradresse von shmat: %p\n",shmat(shmid_g, NULL, 0));
  struct SHM_data* shm_pointer = shmat(shmid, NULL, 0);
  char *pipeBuffer=think_new(shm_pointer);
  //printf(" Thinker berechneter Zug: %s\n ",pipeBuffer);
  //printf("pipebuffer: %s \n", pipeBuffer);
	if((write(pipeFd[1], pipeBuffer, PIPE_BUF))<=0){
       perror("Fehler beim schreiben des Spielzugs in die pipe, BRAIN");
       return -1;
    }
  //printf("Spielzug in die Pipe geschrieben, THINKER \n");
  free(pipeBuffer);
  return 0;
}

//Capture Spielzug an Connector schicken / in die Pipe schreiben
	short sendCaptureMove(){
		struct SHM_data* shm_pointer = shmat(shmid, NULL, 0);
		char *pipeBuffer = capture(shm_pointer);

	if((write(pipeFd[1], pipeBuffer, PIPE_BUF))<=0){
		perror("Fehler beim schreiben des CAPTURE Spielzugs in die pipe, BRAIN");
		return -1;
    }

	free(pipeBuffer);
	return 0;
}

static void signalHandlerThinker(int signalNum){
  //printf("Signalhandler go?\n");
  if(signalNum==SIGUSR1){
    //printf("Signal SIGUSR1 angekommen\n");
	sendMove();
  }
  if(signalNum==SIGUSR2){
    //printf("Signal SIGUSR2 angekommen\n");
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
			//printf("Signal an Thinker gesendet,erster spielzug, CONNECTOR \n");
			//sleep(1); //TODO ist sleep hier notwendig ?
			//Aus der Pipe den Spielzug lesen
			if((read (pipeFd[0], movePipe, PIPE_BUF)) >0){
			//printf("Spielzug aus Pipe gelesen: %s \n", movePipe);
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
			//printf("Signal an Thinker gesendet,erster spielzug, CONNECTOR \n");
			//sleep(1); //TODO ist sleep hier notwendig ?
			//Aus der Pipe den Spielzug lesen
			if((read (pipeFd[0], movePipe, PIPE_BUF)) >0){
			//printf("Spielzug aus Pipe gelesen: %s \n", movePipe);
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
  //printf("\nStarte fork_thinker_connector\n");

  //Fork Variablen
  pid_t pid;
  int sockfd;

  //Pipe BUFFER
  char *movePipe=malloc(PIPE_BUF);

  //Erstellung der Pipe, muss vor Fork geschehen
  if (pipe(pipeFd) < 0) {
      perror ("fork_thinker_connector(): Fehler bei Erstellung der Pipe");
      return -1;
   }
   else{
     //printf("fork_thinker_connector(): Created pipe successfully");
   }

	//int shmid;
	size_t shm_size = sizeof(struct SHM_data);
	shmid = shmget(IPC_PRIVATE, shm_size, IPC_CREAT | 0666);
	if (shmid >= 0) {
	struct SHM_data* shm_p = shmat(shmid,  0, 0);
	if (shm_p ==(void *)-1) {
	perror("shmat failed"); 	}
	else {
	shmdt(shm_p);
	}
	}
	else
	{
	/* shmget lief schief */
	perror("shmget failed");
	return -1;
}

	//FORK
  switch(pid = fork()){

	  //ERROR
    case -1: perror("fork_thinker_connector(): Fehler bei fork\n");
      return -1;
      break;

	  //>>=======CONNECTOR=======<<
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
        //printf("\nfork_thinker_connector(): initConnect success\n");
			}

	  struct SHM_data* shm_pointer = shmat(shmid, NULL, 0);;
	  printf("-Start performConnection-\n");

	//Prologphase und senden des ersten THINKING Befehls falls Server move sendet
    if(performConnection(sockfd, shm_pointer) == OKTHINK) {
      //Signal an Thinker senden, erster spielzug des spiels
     send_signal(sockfd, MOVE, movePipe);
  	}




printf("-Ab hier switch case-\n");

	  while(1){
		 // n--;
		  switch(maintainConnection(sockfd, shm_pointer)){
			case MOVE:
				if(shm_pointer->capture_status == 0){
				send_signal(sockfd, MOVE, movePipe);
				}
				else if(shm_pointer->capture_status == 1){
					send_signal(sockfd, CAPTURE, movePipe);
				}
				else if(shm_pointer->capture_status == 2){
					//TODO: 2 Spielsteine werfen
					char *serverResponse=malloc(sizeof(char)*MES_LENGTH_SERVER);
					send_signal(sockfd, CAPTURE, movePipe);
					if(write(sockfd, THINKING_MSG, (int)strlen(THINKING_MSG)) <0){
						perror("Fehler beim senden von THINKING");
					}
					printf("C: %s", THINKING_MSG);

				//receive OKTHINK
				if(read(sockfd, serverResponse, sizeof(char)*MES_LENGTH_SERVER) < 0){
					perror("Fehler beim empfangen von OKTHINK");
				};
					printf("\nS: %s",serverResponse);

					read_piecelist(shm_pointer, serverResponse);
					send_signal(sockfd, CAPTURE, movePipe);
					free(serverResponse);
				}
				break;

			case WAIT: break;

			case MOVEOK: break;

			case GAMEOVER:
			printf("S: GAMEOVER");
			close(close(pipeFd[0]));
			exit(0);
			break;

			case ERROR:
			printf("CASE ERROR");
			close(close(pipeFd[0]));
			exit(0);
			break;
		  }

	  }
	  close(close(pipeFd[0]));
			exit(0);
	  //>>=======THINKER=======<<
    default: printf("Elternprozess(Thinker) mit der id %d und der Variable pid = %d. MeinElternprozess ist: %d\n", getpid(), pid, getppid());

	shmat(shmid, NULL, 0);
			//Leseseite der Pipe schliessen
			close (pipeFd[0]);

			//Signalhandler fuer Connector Signal definieren
			//http://pubs.opengroup.org/onlinepubs/009695399/functions/sigaction.html muss code nochmal pruefen
			struct sigaction sa;
			sa.sa_handler = signalHandlerThinker;
			sigemptyset(&sa.sa_mask);
			sa.sa_flags = SA_RESTART;
			//printf("Signalhandler definiert THINKER \n");

			if(sigaction(SIGUSR1, &sa , NULL ) == 0){ //Bei success 0, sonst -1
				//printf("sigaction sucess THINKER\n");
			}
			else{
				perror("sigaction groesser Null, Fehler ???");
			}
			if(sigaction(SIGUSR2, &sa , NULL ) == 0){ //Bei success 0, sonst -1
				//printf("sigaction sucess THINKER\n");
			}
			else{
				perror("sigaction groesser Null, Fehler ???");
			}
			wait(NULL);
      break;
  }
  free(movePipe);
  return 0;
}

int main(int argc, char *argv[]){
  short paras;
  char parGameId[BUFFER_SIZE];
  char parPlayerNumber[BUFFER_SIZE];
  char parConfigFileLocation[BUFFER_SIZE];

  //Auslesen der Parameter
  while( (paras = getopt(argc, argv, "g:p:c")) != -1){
    switch(paras){
      case 'g':
                printf("optarg g: \"%s\"\n",optarg);
                if(optarg == NULL){
                  strcpy(parGameId," ");
                }else{
                  if((strstr(optarg, "-")) != NULL){
                    strcpy(parGameId," ");
                  }else{
                    strcpy(parGameId,optarg);
                  }
                }
                break;
      case 'p':
                printf("\nplaynumpara: %s\n",optarg);
                if(optarg == NULL){
                  strcpy(parPlayerNumber," ");
                }
                else{
                  strcpy(parPlayerNumber,optarg);
                }
                printf("\nplayernumber: %s \n", parPlayerNumber);
                break;
      case 'c':
                printf("optarg c: \"%s\"\n",optarg);
                if( (optarg == NULL) || (strcmp(optarg," ") == 0) || (strcmp(optarg,"") == 0) ){
                  strcpy(parConfigFileLocation," ");
                }
                else{
                  strcpy(parConfigFileLocation,optarg);
                }
                break;
      default:
                perror("Fehler: ungueltiger Parameter, THINKCON");
    }
  }
  if( (strcmp(parPlayerNumber,"1") == 0) || (strcmp(parPlayerNumber,"2") == 0) ){
  }
  else{
    strcpy(parPlayerNumber," ");
  }
  if( (strcmp(parConfigFileLocation," ") == 0) || (strcmp(parConfigFileLocation,"") == 0) ){
    strcpy(parConfigFileLocation," ");
  }
  if(read_configfile(parGameId, parPlayerNumber, parConfigFileLocation) == -1){
    perror("Terrible Failure in readConfig.c , THINKCON");
    return -1; //TODO vllt einfach iwleche standardwerte assignen, statt abzustuerzen ???
  }

	drawField();
	parseMove("A1:A2",0);
   parseMove("A3:A4",1);
   parseMove("B1:C1",0);
   parseMove("C3:C2",1);
   parseMove("C1:C2",0);
	fork_thinker_connector();
return 0;
}
