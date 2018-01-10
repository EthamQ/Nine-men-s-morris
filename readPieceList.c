#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>




char testread[] = "+ CAPTURE 0\n+ PIECELIST 2,9\n+ PIECE0,0 A1\n+ PIECE0,1 A\n+ PIECE0,2 A3\n+ PIECE0,3 A\n+ PIECE0,4 A\n+ PIECE0,5 A\n+ PIECE0,6 A\n+ PIECE0,7 A\n+ PIECE0,8 A\n+ PIECE1,0 A\n+ PIECE1,1 A\n+ PIECE1,2 A\n+ PIECE1,3 A\n+ PIECE1,4 A\n+ PIECE1,5 A\n+ PIECE1,6 A4\n+ PIECE1,7 A\n+ PIECE1,8 C0\n";

int read_piecelist_hidden(char* piecelist, char *search, int search_length, int startposition, char* status){
	printf("Looking for: %s\n", search);
	//printf("start_length: %i\n", search_length);
	int array_length = sizeof(testread) / sizeof(testread[0]);
	//printf("array_length: %i\n", array_length);
	int pos_text = 0;
	int pos_search = 0;
    for (pos_text = startposition; pos_text < array_length; pos_text++){
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
    // gefunden, man befindet sich beim T von PIECELIST
    printf("Position text = %i, an dieser Stelle steht ein [%c][%c][%c][%c]\n", pos_text, testread[pos_text], testread[pos_text+1], testread[pos_text+2], testread[pos_text+3]);
	status[0] = testread[pos_text+3];
	status[1] = testread[pos_text+4];
	return pos_text;

}


//"Spielfeld" ausgeben
void print(int fieldd[3][8]){
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
	

int field[3][8];
//TODO: fill array in shared memory
	void fill_array(int player, char status[2]){
		printf("value of empty: %i\n", status[1]);
		printf("value of char 5: %i\n", '5');
		int zahl = status[1] - '0';
		if(zahl >= 0 && zahl <= 8){
			if(status[0] == 'A'){
				field[0][zahl] = player;
			}
			if(status[0] == 'B'){
				field[1][zahl] = player;
			}
			if(status[0] == 'C'){
				field[2][zahl] = player;
			}
			print(field);
			return;
		}
		printf("2. Zeichen leer\n");
		
	}
	
	
	void read_piecelist(){
	int pos = 0;
	int n = 0;
	char status[2];
	while(n<9){
	pos = read_piecelist_hidden(testread, "PIECE0,",sizeof("PIECE0,")-1, pos, status);
	fill_array(1, status);
	printf("PIECE %c status: %c%c\n", testread[pos+1], status[0], status[1]);
	n++;
	}
	n = 0;
	while(n<9){
	pos = read_piecelist_hidden(testread, "PIECE1,",sizeof("PIECE1,")-1, pos, status);
	fill_array(2, status);
	printf("PIECE %c status: %c%c\n", testread[pos+1], status[0], status[1]);
	n++;
	}
	
	}
int main(){
	//read_piecelist(testread, "PIECELIST",sizeof("PIECELIST")-1, 0);
	read_piecelist();
	
}