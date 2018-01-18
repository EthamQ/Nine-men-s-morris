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

//included Headerfiles
#include <sys/ipc.h>
#include <sys/sem.h>
#include "performConnection.h"
#include "shm_data.h"
#include "debugging.h"
#include "config_header.h"
#include "brain.h"
#include "maintainConnection.h"
#include "constants.h"

int pipeFd[2];
int shmid;

//Connect to the server and return the socket file descriptor
int initConnect(){
      int sockfd;
      int rv;

      char gameKindNameBuffer[BUFFER_SIZE];
      char portNumberBuffer[BUFFER_SIZE];
      char hostNameBuffer[BUFFER_SIZE];
      strcpy(gameKindNameBuffer,confiConst.gameKindName);
      strcpy(portNumberBuffer,confiConst.portNumber);
      strcpy(hostNameBuffer,confiConst.hostName);

      struct addrinfo hints, *servinfo, *p;
      memset(&hints,0, sizeof hints);
      hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
      hints.ai_socktype = SOCK_STREAM;

      if ((rv = getaddrinfo(hostNameBuffer, portNumberBuffer, &hints, &servinfo)) != 0) {
      	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
      	return ERROR;
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
      return ERROR;
    }

  freeaddrinfo(servinfo); // all done with this structure

return sockfd;
}

//Normalen Spielzug an Connector schicken / in die Pipe schreiben
short sendMove(){
  struct SHM_data* shm_pointer = shmat(shmid, NULL, 0);
  char *pipeBuffer=think_new(shm_pointer);
  if((write(pipeFd[1], pipeBuffer, PIPE_BUF))<=0){
       perror("Fehler beim schreiben des Spielzugs in die pipe, BRAIN");
       return ERROR;
    }

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
  if(signalNum==SIGUSR1){
	sendMove();
  }
  if(signalNum==SIGUSR2){
	sendCaptureMove();
  }
}


	//Sendet entsprechendes Signal an den Thinker und liest dann den Spielzug aus der pipe
	//und sendet ihn an conPlay, der die passende Nachricht an den Server sendet
	void send_signal(int sockfd, int task, char* movePipe){
	if(task == MOVE){
			//SIGUSR1 Signal an den thinker
			if(kill(getppid(),SIGUSR1)<0){
			perror("Fehler bei senden von SIGUSR1 an den Thinker, CONNECTOR");
			}

			if((read (pipeFd[0], movePipe, PIPE_BUF)) >0){
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


			//Aus der Pipe den Spielzug lesen
			if((read (pipeFd[0], movePipe, PIPE_BUF)) >0){
				perror("Spielzug konnte nicht aus der Pipe gelesen werden");
			}
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
  char *movePipe=malloc(PIPE_BUF);

  //Erstellung der Pipe, muss vor Fork geschehen
  if (pipe(pipeFd) < 0) {
      perror ("fork_thinker_connector(): Fehler bei Erstellung der Pipe");
      return ERROR;
   }
   else{
      printf("fork_thinker_connector(): Created pipe successfully");
   }


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
	return ERROR;
}

semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL);


	//FORK
  switch(pid = fork()){

	  //ERROR
    case ERROR: perror("fork_thinker_connector(): Fehler bei fork\n");
      return ERROR;
      break;

	  //>>=======CONNECTOR=======<<

    case 0: printf("Kindprozess(Connector) mit der id %d und der Variable pid = %d. Mein Elternprozess ist: %d\n", getpid(), pid, getppid());

		 struct SHM_data* shm_pointer = shmat(shmid, NULL, 0);

	  shm_pointer->pid_connector = getpid();
	  shm_pointer->pid_thinker = getppid();
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
	 	 printf("-Start performConnection-\n");

	//Prologphase und senden des ersten THINKING Befehls falls Server move sendet
    	if(performConnection(sockfd, shm_pointer) == OKTHINK) {
      		//Signal an Thinker senden, erster spielzug des spiels
     		send_signal(sockfd, MOVE, movePipe);
  	}




printf("-Ab hier switch case-\n");

	  while(1){
		switch(maintainConnection(sockfd, shm_pointer)){
		case MOVE:
				if(shm_pointer->capture_status == 0){
				send_signal(sockfd, MOVE, movePipe);
				}
				else
				       	if(shm_pointer->capture_status == 1){
						send_signal(sockfd, CAPTURE, movePipe);
				}
				else
				       	if(shm_pointer->capture_status == 2){
						char *server_Response=malloc(sizeof(char)*MES_LENGTH_SERVER);
						send_signal(sockfd, CAPTURE, movePipe);
					if(write(sockfd, THINKING_MSG, (int)strlen(THINKING_MSG)) <0){
						perror("Fehler beim senden von THINKING");
					}
					printf("C: %s", THINKING_MSG);

				//receive OKTHINK
					if(read(sockfd, server_Response, sizeof(char)*MES_LENGTH_SERVER) < 0){
						perror("Fehler beim empfangen von OKTHINK");
					};
					printf("\nS: %s",server_Response);

					read_piecelist(shm_pointer, server_Response);
					send_signal(sockfd, CAPTURE, movePipe);
					free(server_Response);
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

	struct SHM_data* shm_pointer_t = shmat(shmid, NULL, 0);
	if (shm_pointer_t ==(void *)-1) {
	perror("shmat failed im thinker");
	}

	printf("\nPID THINKER nach initialisierung: %i\n", shm_pointer->pid_thinker);

	//Leseseite der Pipe schliessen
	close (pipeFd[0]);

	//Signalhandler fuer Connector Signal definieren
	struct sigaction sa;
	sa.sa_handler = signalHandlerThinker;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;


			if(sigaction(SIGUSR1, &sa , NULL ) < 0){ //Bei success 0, sonst -1
				perror("Fehler bei sigaction SIGURS1");
			}
			if(sigaction(SIGUSR2, &sa , NULL ) < 0){ //Bei success 0, sonst -1
				perror("Fehler bei sigaction SIGURS2");
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
                //printf("optarg g: \"%s\"\n",optarg);
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
                //printf("\nplaynumpara: %s\n",optarg);
                if(optarg == NULL){
                  strcpy(parPlayerNumber," ");
                }
                else{
                  strcpy(parPlayerNumber,optarg);
                }
                //printf("\nplayernumber: %s \n", parPlayerNumber);
                break;
      case 'c':
                //printf("optarg c: \"%s\"\n",optarg);
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
