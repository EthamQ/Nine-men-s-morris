#include <unistd.h>

//Spielzug an Connector schicken / in die Pipe schreiben
short sendMove(*char destination){
  char pipeBuffer[5];

  pipeBuffer = &destination;
  int gesendeteBytes = sizeof(pipeBuffer); //der return wert von write ist die anzahl der gesendeten bytes, falls das != der zu sendenden bytes PANIK !

  if ((write (pipeFd[1], pipeBuffer, gesendeteBytes)) != gesendeteBytes) {
       perror("Fehler beim schreiben des Spielzugs in das pipe, BRAIN");
       return -1;
    }
  printf("Spielzug in die Pipe geschrieben, BRAIN");
  }
  //sleep(0.5);
  return 0;
}

short think(){
  if(sendmove() == 0{
  return 0;
  }
  else{
    return -1;
  }
}

}
