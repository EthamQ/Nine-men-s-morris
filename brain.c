#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "drawfield.h" //Siehe Meilenstein 3

char* think(){
	char *move;
	move=malloc(sizeof(move));
  	strcpy(move,"PLAY B1\n");
  //Spielfeld einlesen
    //TODO

  //Thinken
  //move[] = "A1 A2";


 return move;
 //TODO MEMORYLEAK VERHINDERN
}

//Ab hier neue Version von Raphael

#define NUMBER_FIELDS 24
#define NUMBER_STONES 9
#define ZEILEN 3
#define SPALTEN 8
#define EMPTY 0
#define PLAYER1 1
#define PLAYER2 2

int stones;
int stonesA = 0;
int stonesB;



void swap(int field[ZEILEN][SPALTEN], int x, int y, int x_new, int y_new){
	field[x][y] = EMPTY;
	field[x_new][y_new] = PLAYER1;
}

//an ein benachbartes freies Feld ziehen
void move(int field[ZEILEN][SPALTEN]){
	int x;
	int y;
	for(x = 0; x<ZEILEN; x++){
	for(y = 0; y<SPALTEN; y++){
		if(field[x][y] == 1){
				if(field[x][(y+1)%SPALTEN] == 0){
					swap(field, x, y, x, (y+1)%SPALTEN);
					return;
				}
				
			}
		}
}
}



//Macht aus einem x und y Wert einen PLAY command für den Server
void create_PLAY_command(char* pos, int x, int y){
switch(x){
case 0: strcpy(pos, "PLAY A"); break;	
case 1: strcpy(pos, "PLAY B"); break;	
case 2: strcpy(pos, "PLAY C"); break;	
}

char number[10];
sprintf(number, "%i\n", y);
strcat(pos, number);
}

//Wenn weniger als 9 Steine gelegt auf beliebiges freies Feld einen Stein legen
//ansonsten auf ein beliebeiges freies Feld ziehen
//returned passenden PLAY command 
char* think(int field[ZEILEN][SPALTEN]){
	//field = parse MOVE server message
	char* buchstabe;
	char* zahl;
	//random empty position
	if(stonesA < NUMBER_STONES){
		int i;
		int j;
		for(i = 0; i<ZEILEN; i++){
		for(j = 0; j<SPALTEN; j++){
			if(field[i][j] == 0){
				field[i][j] = 1;
				stonesA++;
				char* pos = malloc(15);;
				create_PLAY_command(pos, i, j);
				printf("\n%s", pos);
				return pos;
			}
		}
		}
	}
	
	else{
		if(stonesA >= 3){
			move(field);
		}
		else{
			
		}
	}
}

//"Spielfeld" ausgeben
void print(int field[ZEILEN][SPALTEN]){
	int i;
	int j;
	for(i = 0; i<ZEILEN; i++){
	for(j = 0; j<SPALTEN; j++){
		printf("%i", field[i][j]);
		if(j==(SPALTEN-1)){
			printf("\n");
		}
	}	
	}
}

//Spielfeld initialisieren
void init(int field[ZEILEN][SPALTEN]){
	int i;
	int j;
	for(i = 0; i<3; i++){
	for(j = 0; j<8; j++){
		field[i][j] = EMPTY;
	}
	}
}


//Testaufruf wenn man brain einzeln kompiliert und ausführt
/*
int main(){
	
	int field[ZEILEN][SPALTEN];
	init(field);
	print(field);
	printf("\n\n");
	int i;
	for(i = 0; i<15; i++){
		think(field);
		print(field);
	printf("\n\n");
	}
	*/
	
	
	
}



