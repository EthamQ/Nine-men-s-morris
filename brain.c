#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

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
#define PLAYER_CLIENT 1
#define PLAYER_OPPONENT 2
#define A 0
#define B 1
#define C 2
#define SIZE_PLAY_COMMAND 15

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
	field[x_new][y_new] = PLAYER_CLIENT;
	printf("\n\nNach swap: \n");
	print(field);
}

//speichert den x und y Wert einer neuen freien Position zu der man ziehen kann in dem Übergabearray newPos
//returned 1 wenn eine freie Position gefunden wurde
int free_pos(int field[ZEILEN][SPALTEN], int newPos[2], int x, int y){
	printf("\nFunction free_pos with the position [%i][%i]\n", x, y);
	int check_x;
	int check_y;
	
		//Suche nach freiem Platz auf dem Ring des entsprechenden Buchstabens
			//Position: y+1
			check_y = (y+1)%SPALTEN;
			printf("check if field[%i][%i] == EMPTY | %i\n", x, check_y, (field[x][check_y] == EMPTY));
			if(field[x][check_y] == EMPTY){
				newPos[0] = x;
				newPos[1] = check_y;
				return 1;		
		}
		
			//Position: y-1
			check_y = (y+7)%SPALTEN;
			printf("check if field[%i][%i] == EMPTY | %i\n", x, check_y, (field[x][check_y] == EMPTY));
			if(field[x][check_y] == EMPTY){
			newPos[0] = x;
			newPos[1] = check_y;
			return 1;
		}
		
		//Falls kein freier Platz auf Ring: suche Platz auf anderen Ringen
		//A sucht nach Platz in B Ring
		if(x == A && y != 0 && (y%2 != 0)){
			check_x = x+1;
			printf("check if field[%i][%i] == EMPTY | %i\n", check_x, y, (field[check_x][y] == EMPTY));
			if(field[check_x][y] == EMPTY){
			newPos[0] = check_x;
			newPos[1] = y;
			return 1;
		}
		}

		//B sucht nach Platz in A und C Ring
		if(x == B && y != 0 && (y%2 != 0)){
			//A Ring
			check_x = x-1;
			printf("check if field[%i][%i] == EMPTY | %i\n", check_x, y, (field[check_x][y] == EMPTY));
			if(field[check_x][y] == EMPTY){
			newPos[0] = check_x;
			newPos[1] = y;
			return 1;
			}
			//C Ring
			check_x = x+1;
			printf("check if field[%i][%i] == EMPTY | %i\n", check_x, y, (field[check_x][y] == EMPTY));
			if(field[check_x][y] == EMPTY){
			newPos[0] = check_x;
			newPos[1] = y;
			return 1;
			}
		}
		 
		//C sucht nach Platz in B Ring
		if(x == C && y != 0 && (y%2 != 0)){
			check_x = x-1;
			printf("check if field[%i][%i] == EMPTY | %i\n", check_x, y, (field[check_x][y] == EMPTY));
			if(field[check_x][y] == EMPTY){
			newPos[0] = check_x;
			newPos[1] = y;
			return 1;
		}
		}
	
		
		printf("Es wurde keine freie Position gefunden");
		return 0;	
	}
	


//an ein benachbartes freies Feld ziehen
void move(int field[ZEILEN][SPALTEN], int FROM[2], int TO[2]){
	printf("\nFunction move:\n");
	int x;
	int y;

	//bewegt zufälligen Client Stein
	while(1){
		x = rand()%ZEILEN;
		y = rand()%SPALTEN;
		if(field[x][y] == PLAYER_CLIENT){
			int new_position[2];
			int free = free_pos(field, new_position, x, y);
			if(free){
			swap(field, x, y, new_position[0], new_position[1]);
			//Füllen der Arrayposition für think_new()
			FROM[0] = x;
			FROM[1] = y;
			TO[0] = new_position[0];
			TO[1] = new_position[1];
			return;
			}
			}
}
}



//Macht aus einem x und y Wert einen PLAY command für den Server, zb PLAY A1
void create_PLAY_command(char* pos, int x, int y){
switch(x){
case A: strcpy(pos, "PLAY A"); break;	
case B: strcpy(pos, "PLAY B"); break;	
case C: strcpy(pos, "PLAY C"); break;	
}

char number_extension[SIZE_PLAY_COMMAND];
//int zu char
sprintf(number_extension, "%i\n", y);
strcat(pos, number_extension);
}

//Macht aus jeweils 2 x und y Werten einen PLAY command(von einem Feld auf ein anderes ziehen) für den Server, zb PLAY A1:A2
void create_MOVE_command(char* pos, int x, int y, int x_new, int y_new){
	switch(x){
	case A: strcpy(pos, "PLAY A"); break;	
	case B: strcpy(pos, "PLAY B"); break;	
	case C: strcpy(pos, "PLAY C"); break;	
	}
	char number_extension[SIZE_PLAY_COMMAND];
	//int zu char
	sprintf(number_extension, "%i", y);
	strcat(pos, number_extension);
	
	switch(x_new){
	case A: strcat(pos, ":A"); break;	
	case B: strcat(pos, ":B"); break;	
	case C: strcat(pos, ":C"); break;	
	}
	
	char number_extension_new[SIZE_PLAY_COMMAND];
	//int zu char
	sprintf(number_extension_new, "%i\n", y_new);
	strcat(pos, number_extension_new);
}

//Wenn weniger als 9 Steine gelegt auf beliebiges freies Feld einen Stein legen
//ansonsten auf ein beliebeiges freies Feld ziehen
//returned passenden PLAY command 
char* think_new(int field[ZEILEN][SPALTEN]){
	//play_command den man returned
	char* play_command = malloc(SIZE_PLAY_COMMAND);
	//TODO: field = parse MOVE server message
	//stone on random empty position
	if(stonesA < NUMBER_STONES){
		/* Intializes random number generator */
		time_t t;
		srand((unsigned) time(&t));
			while(1){
			int x = (rand() % ZEILEN);
			int y = (rand() % SPALTEN);
			if(field[x][y] == 0){
				field[x][y] = 1;
				stonesA++;
				create_PLAY_command(play_command, x, y);
				printf("\n%s\n", play_command);
				print(field);
				printf("\n");
				return play_command;
			}
			}
	}
		
	
	else{
		int FROM[2];
		int TO[2];
		if(stonesA >= 3){
			move(field, FROM, TO);
			create_MOVE_command(play_command, FROM[0], FROM[1], TO[0], TO[1]);
			printf("\n%s\n", play_command);
			return play_command;
		}
		else{
		}
	}
	return play_command;
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