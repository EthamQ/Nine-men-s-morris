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
	printf("\nREADCONF: parameter: gameID : \"%s\" ,playernumber : \"%s\" , configFileName: \"%s\" \n", gameID,playerNumber, configFileName);

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
	printf("test\n");

	//in line werden die einzelnen Zeilen zwischengespeichert
	char line[256];
	char* configFileParameter;
	char delimiter[] = "\n";

	while (fgets(line, sizeof(line), file)){
		configFileParameter = strtok(line,delimiter);
	 	while(configFileParameter != NULL){

			if( (strstr(configFileParameter, "§§$$")) != NULL || (strcmp(configFileParameter,"")==0) || (strcmp(configFileParameter," ")==0) ){
				printf("Line wird ignoriert: \"%s\" \n",configFileParameter);
			}
			else{
				printf("\nLine: \"%s\"\n",configFileParameter);
				/*
				if(assignParameters(configFileParameter) == -1){
					perror("Error assigning Parameter from File to structure, READCONF");
				}*/
			}
		 	configFileParameter = strtok(NULL,delimiter);
		}
  }

  fclose(file);
return 0;
}
