#include <fcntl.h> //Fuer Pipe
#include <signal.h> //Fuer Signal Connector->Thinker
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#include "shm_data.h"
#include "brain.h"

//Spielzug an Connector schicken / in die Pipe schreiben
int sendMove(){
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
    printf("Signal empfangen. THINKER");
    sendMove();

  }
}
int thinker(){
	printf("Thinker aufgerufen");
      //Code for Thinker
      //Nicht in Meilenstein 2 implementiert

			
			

			//Signalhandler fuer Connector Signal definieren
			//http://pubs.opengroup.org/onlinepubs/009695399/functions/sigaction.html muss code nochmal pruefen
			struct sigaction sa;
			sa.sa_handler = signalHandlerThinker;
			sigemptyset(&sa.sa_mask);
			sa.sa_flags = SA_RESTART;
			printf("Signalhandler definiert THINKER \n");



			if(sigaction(SIGUSR1, &sa , NULL ) > 0){///!!!!!!
				printf("sigaction sucess THINKER");
			}
			else{
				perror("sigaction fehler ???");
			}

			wait(NULL);
      
  

		return 0;

}