#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//included Headerfiles
#include "shm_data.h"
#include "drawfield.h"

int available_pieces = 0;






/*bekommt die Servernachricht, den char nach dem gesucht wird, ab welcher Stelle in der piecelist gesucht werden soll
und ein char array status wo es den Status der einzelnen pieces reinschreiben kann
Geht immer bis zum letzten char von char* search und von dort kann man dann auf die nachfolgenden Stellen in der piecelist
zugreifen wo die relevanten Informationen stehen
*/
int read_piecelist_hidden(char* piecelist, char *search, int startposition, char* status){
	int array_length = strlen(piecelist);
	int search_length = strlen(search);
	int pos_text = 0;
	int pos_search = 0;
    for (pos_text = startposition; pos_text < array_length; pos_text++){
        if(piecelist[pos_text] == search[pos_search]){
            pos_search++;
            if(pos_search == search_length){
                // match
               
		break;
            }
		}
        else{
           pos_text -= pos_search;
           pos_search = 0;
        }
    }
   
   //Suchwort gefunden, pos_text befindet sich  beim letzten char des Suchbegriffs
	status[0] = piecelist[pos_text+3];
	status[1] = piecelist[pos_text+4];
	return pos_text;

}


int read_from_piecelist(char* piecelist, int task){
	int array_length = strlen(piecelist);
	char *search;
	
	switch(task){
		case CAPTURE: search = "CAPTURE ";
		break;
		case NUMBER_OF_PLAYERS: search = "TOTAL ";
		break;
		case PLAYER_NUMBER: search = "YOU ";
		break;
	}
	int search_length = strlen(search);
	int pos_text;
	int pos_search = 0;
    for (pos_text = 0; pos_text < array_length; pos_text++){
        if(piecelist[pos_text] == search[pos_search]){
            pos_search++;
            if(pos_search == search_length){
                // match
				break;
            }
		}
        else{
           pos_text -= pos_search;
           pos_search = 0;
        }
    }
	return piecelist[pos_text+1] - '0';
}

//funktioniert wie read_piecelist_hidden nur mit kleinen Vereinfachungen, da es nur nach "CAPTURE " sucht
//returned den Wert, der nach CAPTURE steht, also 0 oder 1 (bei 1 muss man einen Stein des Gegners werfen)
int read_capture_status(char* piecelist){
	return read_from_piecelist(piecelist, CAPTURE);
}

int read_player_number(char* piecelist){
	return read_from_piecelist(piecelist, PLAYER_NUMBER);
}


int read_number_of_players(char* piecelist){
	return read_from_piecelist(piecelist, NUMBER_OF_PLAYERS);
}


int read_player_name(char* piecelist, struct SHM_data* shm_pointer){
	int array_length = strlen(piecelist);
	char *search = "YOU ";
	
	int search_length = strlen(search);
	int pos_text;
	int pos_search = 0;
    for (pos_text = 0; pos_text < array_length; pos_text++){
        if(piecelist[pos_text] == search[pos_search]){
            pos_search++;
            if(pos_search == search_length){
                // match
				break;
            }
		}
        else{
           pos_text -= pos_search;
           pos_search = 0;
        }
    }
	int i = 0;
	pos_text = pos_text+3;
	while(piecelist[pos_text+1] != '+'){
		
		shm_pointer->client.spielername[i++] = piecelist[pos_text++];
	}
	printf("name: %s\n", shm_pointer->client.spielername);
	return 0;
}

int read_ready_status(char* piecelist){
	char delimiter[] = "+";
	char *copy = malloc(strlen(piecelist));
	strcpy(copy,piecelist);
	char *ptr;
// initialisieren und ersten Abschnitt erstellen
ptr = strtok(copy, delimiter);
	//printf("Abschnitt gefunden: %s\n", ptr);
int i =0;
while(i<2) {

	// naechsten Abschnitt erstellen
 	ptr = strtok(NULL, delimiter);
		//printf("Abschnitt gefunden: %s\n", ptr);
	i++;
}
printf("char: %c\n", ptr[strlen(ptr)-2]);
//free(copy);
return ptr[strlen(ptr)-2] - '0';
}
//"Spielfeld" ausgeben
void printt(int fieldd[3][8]){
	int i;
	int j;
	for(i = 0; i<3; i++){
	for(j = 0; j<8; j++){
		printf("%i", fieldd[i][j]);
		if(j==(8-1)){
			printf("\n");
		}
	}	
	}
}
	
	//Liest den Status eines Steins ein und schreibt ihn ggf in das shared memory Spielfeld wenn er gesetzt ist
	void fill_array(int player, char status[2], struct SHM_data* shm_pointer){
		
		//Umwandeln char zu int
		int zahl = status[1] - '0';
		
		//Falls nur ein A hinter einem piece vom Client und keine Zahl dann ist es available
		if(player == PLAYER_CLIENT && status[0] == 'A' && (zahl < 0 || zahl > 8)){
			available_pieces++;
		}
		
		//Falls ASCII Wert des chars eine Dezimalzahl zwischen 0 und 7 ist wurde der Spielstein gesetzt
		//und wird in shared memory geschrieben
		if(zahl >= 0 && zahl < 8){
			if(status[0] == 'A'){
				shm_pointer->field[0][zahl] = player;
				//printf("Player: %i - set[A][%i]",player,  zahl);
			}
			if(status[0] == 'B'){
				shm_pointer->field[1][zahl] = player;
				//printf("Player: %i - set[B][%i]",player, zahl);
			}
			if(status[0] == 'C'){
				shm_pointer->field[2][zahl] = player;
				//printf("Player: %i - set[C][%i]",player, zahl);
			}	
		}		
	}
	
	//Alle Stellen auf 0(=EMPTY) setzen
	void resetArray(struct SHM_data* shm_pointer){
		int i, j;
		for(i = 0; i<3 ; i++){
			for(j = 0; j<8 ; j++){
			shm_pointer->field[i][j] = EMPTY;
		}
	}
	}
	
	//Liest alle Spielsteine, die gesetzten Steine und den capture status aus der Servernachricht
	//und schreibt alles in die shared memory
	void read_piecelist(struct SHM_data* shm_pointer, char* piecelist){
		//Alle Stellen im Spielfeld auf 0 setzen
		resetArray(shm_pointer);
		/*printf("Field wurde resettet: \n");
		printt(shm_pointer->field);
		printf("\n");
		*/
		
		
		
		
	
		//Hier wird immer der Status der einzelnen pieces zwischengespeichert
		char status[2];
		status[0] = 'E';
		status[1] = 'E';
		
		shm_pointer->anzahl_spieler =  read_number_of_players(piecelist);
		printf("Number of players: %i\n", shm_pointer->anzahl_spieler);
		
		//which player are we?
		 int plNR = read_player_number(piecelist);
		 if(plNR == 0 || plNR == 1){
		 shm_pointer->client.spielernummer = plNR;
		 shm_pointer->client.spielernummer = 1-plNR;		
		 shm_pointer->opponent.flag_registriert = read_ready_status(piecelist);		 
		 read_player_name(piecelist, shm_pointer);
		 //printf("ready %i\n", shm_pointer->client.flag_registriert);
		 }
		 printf("\nYOU: %i\n", shm_pointer->client.spielernummer);
		 
		 int playerA;
		 int playerB;
		 if(shm_pointer->client.spielernummer == 0){
			 playerA = PLAYER_CLIENT;
			 playerB = PLAYER_OPPONENT;
		 }
		 else if(shm_pointer->client.spielernummer == 1){
			 playerA = PLAYER_OPPONENT;
			 playerB = PLAYER_CLIENT;
		 }

		 
		 //Zählvariablen
		int pos = 0;
		int n = 0;

		//9 Pieces Spieler A
		while(n<9){
		pos = read_piecelist_hidden(piecelist, "PIECE0,", pos, status);
		fill_array(playerA, status, shm_pointer);
		n++;
		}
	
		//9 Pieces Spieler B
		n = 0;
		while(n<9){
		pos = read_piecelist_hidden(piecelist, "PIECE1,", pos, status);
		fill_array(playerB, status, shm_pointer);
		n++;
		}
		printf("\nAvailable pieces: %i\n", available_pieces);
		shm_pointer->used_pieces = NUMBER_STONES - available_pieces; 	
		available_pieces = 0;
	
		//Look at the capture value and write in shared memory
		int capture = read_capture_status(piecelist);
		if(capture != 0 && capture != 1 && capture != 2){
		capture = 0;
		}		
		printf("\nCapture: %i\n", capture);
		shm_pointer->capture_status = capture;
		printf("\nOwn pieces set: %i\n", shm_pointer->used_pieces);
	
		//printf("\nJetzt wurde die Servernachricht eingelesen: \n");
		//printt(shm_pointer->field);
		printf("\n");
		drawField(shm_pointer);
	}
