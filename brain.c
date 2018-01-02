#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "drawfield.h" //Siehe Meilenstein 3

char* think(){
	char *move;
	move=malloc(sizeof(move));
  	strcpy(move,"PLAY A2\n");
  //Spielfeld einlesen
    //TODO

  //Thinken
  //move[] = "A1 A2";


 return move;
 //TODO MEMORYLEAK VERHINDERN
}
