#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "constants.h"
#include "drawfield.h" //Siehe Meilenstein 3
#include "shm_data.h"

//Hilfsfunktion um zu sehen wo man sich befindet wenn man Zwischenergebnisse ausprintet
char buchstabe(int b){
	switch(b){
		case A: return 'A'; break;
		case B: return 'B'; break;
		case C: return 'C'; break;
	}
return 'E';
}

//EDIT: CAPTURE Aufforderung des Servers macht Mühlen finden evtl überflüssig
//returned 1 wenn PLAYER_CLIENT eine Mühle auf einem der jeweiligen Ringe hat, sonst 0
int check_rings(int field[ZEILEN][SPALTEN]){
	int counter;
	int x, y;
	//Testen der Ringe A, B und C
	//Stellen 0, 1, 2
	for(x = 0; x<ZEILEN; x++){
		counter = 0;
		//Stellen 0, 1, 2
		for(y = 0; y<3; y++){
			if(field[x][y] == PLAYER_CLIENT){
				//printf("check field[%i][%i] == PLAYER_CLIENT true\n", x, y);
				counter++;
				//printf("counter: %i\n", counter);
			}
			if(counter == 3){
				printf("\n Mühle gefunden auf einem Ring bei %c 0,1,2\n", buchstabe(x));
				return 1;
			}
		}
		counter = 0;
		//Stellen 2, 3, 4
		for(y = 2; y<5; y++){
			if(field[x][y] == PLAYER_CLIENT){
				//printf("check field[%i][%i] == PLAYER_CLIENT true\n", x, y);
				counter++;
				//printf("counter: %i\n", counter);
			}
			if(counter == 3){
				//printf("\n Mühle gefunden auf einem Ring bei [%i][%i]\n", x, y);
				printf("\n Mühle gefunden auf einem Ring bei %c 2,3,4 \n", buchstabe(x));
				return 1;
			}
		}
		counter = 0;
		//Stellen 4, 5, 6
		for(y = 4; y<7; y++){
			if(field[x][y] == PLAYER_CLIENT){
				//printf("check field[%i][%i] == PLAYER_CLIENT true\n", x, y);
				counter++;
				//printf("counter: %i\n", counter);
			}
			if(counter == 3){
				//printf("\n Mühle gefunden auf einem Ring bei [%i][%i]\n", x, y);
				printf("\n Mühle gefunden auf einem Ring bei %c 4,5,6\n", buchstabe(x));
				return 1;
			}
		}
		counter = 0;
		//Stellen 6, 7, 0
		for(y = 6; y<8; y++){
			if(field[x][y] == PLAYER_CLIENT){
				//printf("check field[%i][%i] == PLAYER_CLIENT true\n", x, y);
				counter++;
				//printf("counter: %i\n", counter);
			}
			if(counter == 2){
				if(field[x][0] == PLAYER_CLIENT){
				//printf("\n Mühle gefunden auf einem Ring bei [%i][%i]\n", x, y);
				printf("\n Mühle gefunden auf einem Ring bei %c 6,7,0\n", buchstabe(x));
				return 1;
				}
			}
		}
		}
	return 0;
}

//returned 1 wenn PLAYER_CLIENT eine Mühle zwischen den Ringen hat, sonst 0
int check_between_rings(int field[ZEILEN][SPALTEN]){
	int counter;
	int x;
	int y;
	for(y = 1; y<SPALTEN; y+=2){
		counter = 0;
	for(x = 0; x<ZEILEN; x++){
		if(field[x][y] == PLAYER_CLIENT){
			counter++;
			}
		if(counter == 3){
			printf("\n Mühle gefunden zwischen den Ringen bei [%i][%i]\n", x, y);
			return 1;
		}
	}
	}
	return 0;
}

//returned 1 wenn eine Mühle von PLAYER_CLIENT gefunden wurde
//wahrscheinlich am besten in conPlay aufrufen nach think und dann ggf noch capture befehl senden
int check_muehle(int field[ZEILEN][SPALTEN]){
	if(check_rings(field) || check_between_rings(field)){
		return 1;
	}
	else return 0;
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



//speichert den x und y Wert einer neuen freien Position zu der man ziehen kann in dem Übergabearray newPos
//returned 1 wenn eine freie Position gefunden wurde
int free_pos(int field[ZEILEN][SPALTEN], int newPos[2], int x, int y){
	//printf("\nFunction free_pos with the position [%i][%i]\n", x, y);
	int check_x;
	int check_y;

		//Suche nach freiem Platz auf dem Ring des entsprechenden Buchstabens
			//Position: y+1
			check_y = (y+1)%SPALTEN;
			//printf("check if field[%i][%i] == EMPTY | %i\n", x, check_y, (field[x][check_y] == EMPTY));
			if(field[x][check_y] == EMPTY){
				newPos[0] = x;
				newPos[1] = check_y;
				return 1;
			}

			//Position: y-1
			check_y = (y+7)%SPALTEN;
			//printf("check if field[%i][%i] == EMPTY | %i\n", x, check_y, (field[x][check_y] == EMPTY));
			if(field[x][check_y] == EMPTY){
			newPos[0] = x;
			newPos[1] = check_y;
			return 1;
		}

		//Falls kein freier Platz auf Ring: suche Platz auf anderen Ringen
		//A sucht nach Platz in B Ring
		if(x == A && y != 0 && (y%2 != 0)){
			check_x = x+1;
			//printf("check if field[%i][%i] == EMPTY | %i\n", check_x, y, (field[check_x][y] == EMPTY));
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
			//printf("check if field[%i][%i] == EMPTY | %i\n", check_x, y, (field[check_x][y] == EMPTY));
			if(field[check_x][y] == EMPTY){
			newPos[0] = check_x;
			newPos[1] = y;
			return 1;
			}
			//C Ring
			check_x = x+1;
			//printf("check if field[%i][%i] == EMPTY | %i\n", check_x, y, (field[check_x][y] == EMPTY));
			if(field[check_x][y] == EMPTY){
			newPos[0] = check_x;
			newPos[1] = y;
			return 1;
			}
		}

		//C sucht nach Platz in B Ring
		if(x == C && y != 0 && (y%2 != 0)){
			check_x = x-1;
			//printf("check if field[%i][%i] == EMPTY | %i\n", check_x, y, (field[check_x][y] == EMPTY));
			if(field[check_x][y] == EMPTY){
			newPos[0] = check_x;
			newPos[1] = y;
			return 1;
			}
		}
		//printf("Es wurde keine freie Position gefunden");
		return 0;
}

//an ein benachbartes freies Feld ziehen
void move(int field[ZEILEN][SPALTEN], int FROM[2], int TO[2], int movetype){
	//printf("\nFunction move:\n");
	int x;
	int y;

	switch(movetype){
	//bewegt zufälligen Client Stein
	case NEIGHBOURING:
	while(1){
		x = rand()%ZEILEN;
		y = rand()%SPALTEN;
		if(field[x][y] == PLAYER_CLIENT){
			int new_position[2];
			int free = free_pos(field, new_position, x, y);
			if(free){
				FROM[0] = x;
				FROM[1] = y;
				TO[0] = new_position[0];
				TO[1] = new_position[1];
				return;
			}
		}
	}
	break;

	case RANDOM:
							while(1){
								x = rand()%ZEILEN;
								y = rand()%SPALTEN;
								if(field[x][y] == PLAYER_CLIENT){
									while(1){
										int xR = rand()%ZEILEN;
										int yR = rand()%SPALTEN;
										if(field[xR][yR] == EMPTY){
											FROM[0] = x;
											FROM[1] = y;
											TO[0] = xR;
											TO[1] = yR;
											return;
										}
									}
								}
							}
							break;
	}

}




//Macht aus einem x und y Wert einen PLAY command für den Server, zb PLAY A1
void create_PLAY_command(char* pos, int x, int y){
	switch(x){
		case A: strcpy(pos, "PLAY A");		
		break;
		case B: strcpy(pos, "PLAY B"); 
		break;
		case C: strcpy(pos, "PLAY C"); 
		break;
	}

	char number_extension[SIZE_PLAY_COMMAND];
	//int zu char
	sprintf(number_extension, "%i\n", y); 
	strcat(pos, number_extension);
}

//Macht aus jeweils 2 x und y Werten einen PLAY command(von einem Feld auf ein anderes ziehen) für den Server, zb PLAY A1:A2
void create_MOVE_command(char* pos, int x, int y, int x_new, int y_new){
	switch(x){
	case A: strcpy(pos, "PLAY A"); 
	break;
	case B: strcpy(pos, "PLAY B"); 
	break;
	case C: strcpy(pos, "PLAY C"); 
	break;
	}
	char number_extension[SIZE_PLAY_COMMAND];
	//int zu char
	sprintf(number_extension, "%i", y);
	strcat(pos, number_extension);
	switch(x_new){
		case A: strcat(pos, ":A"); 
		break;
		case B: strcat(pos, ":B"); 
		break;
		case C: strcat(pos, ":C"); 
		break;
	}

	char number_extension_new[SIZE_PLAY_COMMAND];
	//int zu char
	sprintf(number_extension_new, "%i\n", y_new);
	strcat(pos, number_extension_new);
}


int count_number_of(int type, struct SHM_data* shm_pointer){
	int lookfor;
	int counter = 0;
	switch(type){
		case EMPTY:
		lookfor = EMPTY;
		break;
		case PLAYER_CLIENT:
		lookfor = PLAYER_CLIENT;
		break;
		case PLAYER_OPPONENT:
		lookfor = PLAYER_OPPONENT;
		break;
		}

		for(int x = 0; x<ZEILEN; x++){
			for(int y = 0; y<SPALTEN; y++){
				if(shm_pointer->field[x][y] == lookfor){
					counter++;
				}
			}
		}
		return counter;
	}

//Wenn weniger als 9 Steine gelegt auf beliebiges freies Feld einen Stein legen
//ansonsten auf ein beliebeiges freies Feld ziehen
//returned passenden PLAY command
char* think_new(struct SHM_data* shm_pointer){
	if(shm_pointer->flag_think == 0){
		exit(-1);
	}
	//printf("think new hat folgenden Pointer erhalten: %p", shm_pointer);
	//play_command den man returned
	

	//stone on random empty position if not every stone is placed yet
	if(shm_pointer->used_pieces < NUMBER_STONES){
		char* play_command = malloc(SIZE_PLAY_COMMAND);
		/* Intializes random number generator */
		time_t t;
		srand((unsigned) time(&t));
		while(1){
			int x = (rand() % ZEILEN);
			int y = (rand() % SPALTEN);
			if(shm_pointer->field[x][y] == EMPTY){
				//print(shm_pointer->field);
				create_PLAY_command(play_command, x, y);
				shm_pointer->field[x][y] = PLAYER_CLIENT;
				printf("Client ist am Zug\n");
				printf("Client legt einen Stein auf %c%i\n",buchstabe(x),y);	
				return play_command;
			}
		}
	}
	else{
		char *move_command = malloc(SIZE_MOVE_COMMAND);
		int FROM[2];
		int TO[2];
		//int number =3;
		int number = count_number_of(PLAYER_CLIENT, shm_pointer);
		//printf("\nJetzt alle Spielsteine gesetzt. So viele Spielsteine sind noch auf dem Feld: %i\n", number);
		if(number > 3){
			move(shm_pointer->field, FROM, TO, NEIGHBOURING);
			create_MOVE_command(move_command, FROM[0], FROM[1], TO[0], TO[1]);
			shm_pointer->field[FROM[0]][FROM[1]]= EMPTY;
			shm_pointer->field[TO[0]][TO[1]]= PLAYER_CLIENT;
			printf("Client ist am Zug\n");
			printf("Client bewegt einen Stein von %c%i zu %c%i\n",buchstabe(FROM[0]),FROM[1],buchstabe(TO[0]),TO[1]);
			return move_command;
		}
		else if(number == 3){
		move(shm_pointer->field, FROM, TO, RANDOM);
		create_MOVE_command(move_command, FROM[0], FROM[1], TO[0], TO[1]);
		shm_pointer->field[FROM[0]][FROM[1]]= EMPTY;
		shm_pointer->field[TO[0]][TO[1]]= PLAYER_CLIENT;
		printf("Client ist am Zug\n");
		printf("Client bewegt einen Stein von %c%i zu %c%i\n",buchstabe(FROM[0]),FROM[1],buchstabe(TO[0]),TO[1]);			
		return move_command;
		}
	}
	return NULL;
	
}

	//Wird aufgerufen wenn ein Stein des Gegner geschmissen werden soll
	char* capture(struct SHM_data* shm_pointer){
		if(shm_pointer->flag_think == 0){
		exit(-1);
	}
		char* play_command = malloc(SIZE_PLAY_COMMAND);
		while(1){
			int x = (rand() % ZEILEN);
			int y = (rand() % SPALTEN);
			if(shm_pointer->field[x][y] == PLAYER_OPPONENT){
				create_PLAY_command(play_command, x, y);
				shm_pointer->field[x][y] = EMPTY;
				printf("\nCapture command: %s\n", play_command);
				printf("Client ist am Zug\n");
				printf("Client wirft gegnerischen Stein bei %c%i\n",buchstabe(x),y);
				return play_command;
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
