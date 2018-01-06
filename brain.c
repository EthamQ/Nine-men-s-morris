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
#define A 0
#define B 1
#define C 2

int stones;
int stonesA = 0;
int stonesB;


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

void swap(int field[ZEILEN][SPALTEN], int x, int y, int x_new, int y_new){
	printf("\nFunction swap: [%i][%i] with [%i][%i]\n", x, y, x_new, y_new);
	printf("\nVor swap: \n");
	print(field);
	field[x][y] = EMPTY;
	field[x_new][y_new] = PLAYER1;
	printf("\n\nNach swap: \n");
	print(field);
}

//speichert den x und y Wert einer neuen freien Position zu der man ziehen kann in dem Übergabearray newPos
//returned 1 wenn eine freie Position gefunden wurde
int free_pos(int field[ZEILEN][SPALTEN], int newPos[2], int x, int y){
	printf("\nFunction free_pos with the position [%i][%i]\n", x, y);
	int check_y;
		//Suche nach freiem Platz auf dem Ring des entsprechenden Buchstabens
			//Position: y+1
			check_y = (y+1)%8;
			printf("check if field[%i][%i] == EMPTY | %i\n", x, (y+1)%8, (field[x][check_y] == EMPTY));
			if(field[x][check_y] == EMPTY){
				newPos[0] = x;
				newPos[1] = check_y;
				return 1;		
		}
		
			//Position: y-1
			check_y = (y+7)%8;
			printf("check if field[%i][%i] == EMPTY | %i\n", x, (y+7)%8, (field[x][check_y] == EMPTY));
			if(field[x][check_y] == EMPTY){
			newPos[0] = x;
			newPos[1] = check_y;
			return 1;
		}
		return 0;	
	}
	


//an ein benachbartes freies Feld ziehen
void move(int field[ZEILEN][SPALTEN]){
	printf("\nFunction move:\n");
	int x;
	int y;
	for(x = 0; x<ZEILEN; x++){
	for(y = 0; y<SPALTEN; y++){
		if(field[x][y] == 1){
			int new_position[2];
			int free = free_pos(field, new_position, x, y);
			if(free){
			swap(field, x, y, new_position[0], new_position[1]);
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
char* think_new(int field[ZEILEN][SPALTEN]){
	//Position die man returned
	char* pos = malloc(15);
	//field = parse MOVE server message
	//random empty position
	if(stonesA < NUMBER_STONES){
		int i;
		int j;
		for(i = 0; i<ZEILEN; i++){
		for(j = 0; j<SPALTEN; j++){
			if(field[i][j] == 0){
				field[i][j] = 1;
				stonesA++;
				create_PLAY_command(pos, i, j);
				printf("\n%s\n", pos);
				print(field);
				printf("\n");
				return pos;
			}
		}
		}
	}
	
	else{
		if(stonesA >= 3){
			move(field);
			//TODO: char *pos mit richtigem command füllen
			return pos;
		}
		else{
		}
	}
	return pos;
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
		think_new(field);
		//print(field);
	printf("\n\n");
	}
	
}
*/