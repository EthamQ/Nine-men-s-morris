#include <unistd.h>
#include <stdio.h>
#include <string.h>




short think(){
  string move;
  //Spielfeld einlesen
    //TODO

  //Thinken
  move = "A1 A2";

  //Move senden
  if(sendmove(move) == 0){
    return 0;
  }
  else{
    return -1;
  }
//  return -1;
}

}
