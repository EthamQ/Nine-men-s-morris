#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "shm_data.h"
#include "constants.h"

int available_pieces = 0;




char testread[] = "+ CAPTURE 0\n+ PIECELIST 2,9\n+ PIECE0,0 A1\n+ PIECE0,1 A\n+ PIECE0,2 A3\n+ PIECE0,3 A\n+ PIECE0,4 A\n+ PIECE0,5 A\n+ PIECE0,6 A\n+ PIECE0,7 A\n+ PIECE0,8 A\n+ PIECE1,0 A\n+ PIECE1,1 A\n+ PIECE1,2 A\n+ PIECE1,3 A\n+ PIECE1,4 A\n+ PIECE1,5 A\n+ PIECE1,6 A4\n+ PIECE1,7 A\n+ PIECE1,8 C0\n";

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
                //printf("match from %d to %d\n",pos_text,pos_text);
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

//funktioniert wie read_piecelist_hidden nur mit kleinen Vereinfachungen, da es nur nach "CAPTURE " sucht
//returned den Wert, der nach CAPTURE steht, also 0 oder 1 (bei 1 muss man einen Stein des Gegners werfen)
int read_capture_status(char* piecelist){
	int array_length = strlen(piecelist);
	char *search = "CAPTURE ";
	int search_length = strlen(search);
	//printf("array_length: %i\n", array_length);
	int pos_text;
	int pos_search = 0;
    for (pos_text = 0; pos_text < array_length; pos_text++){
		//printf("Check if %c and %c is the same | %i\n", piecelist[pos_text], search[pos_search], (piecelist[pos_text] == search[pos_search]));
		//printf("pos text: %i, pos_search: %i\n", pos_text, pos_search);
        if(piecelist[pos_text] == search[pos_search]){
            pos_search++;
            if(pos_search == search_length){
                // match
                //printf("match from %d to %d\n",pos_text,pos_text);
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

int read_player_number(char* piecelist){
	int array_length = strlen(piecelist);
	char *search = "YOU ";
	int search_length = strlen(search);
	//printf("array_length: %i\n", array_length);
	int pos_text;
	int pos_search = 0;
    for (pos_text = 0; pos_text < array_length-1; pos_text++){
		//printf("Check if %c and %c is the same | %i\n", piecelist[pos_text], search[pos_search], (piecelist[pos_text] == search[pos_search]));
		//printf("pos text: %i, pos_search: %i\n", pos_text, pos_search);
        if(piecelist[pos_text] == search[pos_search]){
            pos_search++;
            if(pos_search == search_length){
                // match
                //printf("match from %d to %d\n",pos_text,pos_text);
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
		//int field[][] = shm_pointer->field;
		//printf("value of empty: %i\n", status[1]);
		//printf("value of char 5: %i\n", '5');
		int zahl = status[1] - '0';
		
		//Falls nur ein A hinter einem piece und keine Zahl
		if(player == 1 && status[0] == 'A' && (zahl < 0 || zahl > 8)){
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
		printf("Field wurde resettet: \n");
		printt(shm_pointer->field);
		printf("\n");
		
		//Zählvariablen
		int pos = 0;
		int n = 0;
	
		//Hier wird immer der Status der einzelnen pieces zwischengespeichert
		char status[2];
		status[0] = 'E';
		status[1] = 'E';
	
		//which player are we?
		 int plNR = read_player_number(piecelist);
		 if(plNR == 0 || plNR == 1){
		 shm_pointer->player = plNR; 
		 }
		 printf("\nYOU: %i\n", shm_pointer->player);
		 
		 int playerA;
		 int playerB;
		 if(shm_pointer->player == 0){
			 playerA = PLAYER_CLIENT;
			 playerB = PLAYER_OPPONENT;
		 }
		 else if(shm_pointer->player == 1){
			 playerA = PLAYER_OPPONENT;
			 playerB = PLAYER_CLIENT;
		 }
		//9 Pieces Spieler CLIENT
		while(n<9){
		pos = read_piecelist_hidden(piecelist, "PIECE0,", pos, status);
		fill_array(playerA, status, shm_pointer);
		n++;
		}
	
		//available pieces wird hochgezählt in der fill_array methode wenn hinter einem piece nur "A " steht
		//used pieces gibt an wie viele Spielsteine bereits gesetzt wurden und wird in shared memory geschrieben
		printf("\nAvailable pieces: %i\n", available_pieces);
		shm_pointer->used_pieces = NUMBER_STONES - available_pieces; 	
		available_pieces = 0;
	
		//9 Pieces Spieler OPPONENT
		n = 0;
		while(n<9){
		pos = read_piecelist_hidden(piecelist, "PIECE1,", pos, status);
		fill_array(playerB, status, shm_pointer);
		n++;
		}
	
		//Look at the capture value and write in shared memory
		int capture = read_capture_status(piecelist);
		printf("\nCapture: %i\n", capture);
		shm_pointer->capture_status = capture;
		printf("\nOwn pieces set: %i\n", shm_pointer->used_pieces);
	
		printf("\nJetzt wurde die Servernachricht eingelesen: \n");
		printt(shm_pointer->field);
		printf("\n");
	}
