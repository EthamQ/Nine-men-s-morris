#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define paraHOSTNAME 1
#define paraPORT 2
#define paraGAMEKINDNAME 3
#define paraGAMEID 4
#define paraPLAYERNAME 5
#define paraPLAYERNUMBER 6
#define paraVERSION 7

char* isGameidAlreadyDefined = " ";
char* isPlayernumberAlreadyDefined = " ";

int read_configfile(char* gameID, char* playerNumber, char* configFileName){
FILE* file;
char* filepath = configFileName;

	//Pruefen ob parameter gameID und playernumber valide sind
	if(strcmp(gameID," ") != 0){
		isGameidAlreadyDefined = gameID;
	}
	if(strcmp(playerNumber," ") != 0){
		isPlayernumberAlreadyDefined = playerNumber;
	}
//Datei oeffnen und ueberpruefen ob es die datei gibt, falls nicht wird client.conf eingelesen
	file = fopen(filepath, "r");
	if(file == NULL){
		printf("\ninvalid configfilename or path, opening client.conf instead\n\n");
		file=fopen("client.conf","r");
		if(file == NULL){
			perror("client.conf does not exist either D: , READCONF");
			return -1;
		}
		else{
			//printf("opened client.conf successfully, woohoo \n");
		}
	}
	printf("test");
	//in line werden die einzelnen Zeilen zwischengespeichert
	char line[256];
	char* ptr;
	char delimiter[] = "=";
	while (fgets(line, sizeof(line), file)){
		ptr = strtok(line,delimiter);

		 while(ptr != NULL) {
		 	printf("Ausgelesen:%s\n",ptr);
		 	ptr = strtok(NULL,delimiter);
			}
	    }
		printf("\n");

    fclose(file);

return 0;
}
