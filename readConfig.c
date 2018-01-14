#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"

#define paraHOSTNAME 1
#define paraPORT 2
#define paraGAMEKINDNAME 3
#define paraGAMEID 4
#define paraPLAYERNAME 5
#define paraPLAYERNUMBER 6
#define paraVERSION 7

char* isGameidAlreadyDefined = " ";
char* isPlayernumberAlreadyDefined = " ";

char* cleanBlankspace(char* toClean){
	char* bufferChar = toClean;
	char* toReturn;
	char* BlankDeli = " ";

		if((strcmp(bufferChar," ") == 0) || (strcmp(bufferChar,"\0") == 0)){
			return " ";
		}
	while(bufferChar != NULL){
		printf("\nbefore strtok bufferchar: \"%s\"\n",bufferChar);
		toReturn = strtok(bufferChar,BlankDeli);
		if((strcmp(toReturn," "))!=0){	//Alle Leerzeichen wurden entfernt
			return toReturn;
		}
		bufferChar = strtok(NULL,BlankDeli);
	}
	return "ERROR";
}

short tellParam(char* para){
	if( (strcmp(para,"id") == 0) || (strcmp(para,"gameid") == 0) || (strcmp(para,"gameID") == 0) || (strcmp(para,"gameId") == 0) || (strcmp(para,"GAMEID") == 0) || (strcmp(para,"g") == 0) ){
		return paraGAMEID;
	}
	if( (strcmp(para,"port") == 0) || (strcmp(para,"portnumber") == 0) || (strcmp(para,"portNumber") == 0) || (strcmp(para,"PORTNUMBER") == 0)){
		return paraPORT;
	}
	if( (strcmp(para,"host") == 0) || (strcmp(para,"hostName") == 0) || (strcmp(para,"hostname") == 0) || (strcmp(para,"hostNAME") == 0) || (strcmp(para,"HOSTNAME") == 0)){
		return paraHOSTNAME;
	}
	if( (strcmp(para,"gamekindname") == 0) || (strcmp(para,"gameKindName") == 0) || (strcmp(para,"gamekind") == 0) || (strcmp(para,"GAMEKINDNAME") == 0)){
		return paraGAMEKINDNAME;
	}
	if( (strcmp(para,"playername") == 0) || (strcmp(para,"playerNAME") == 0) || (strcmp(para,"playerName") == 0) || (strcmp(para,"PLAYERNAME") == 0)){
		return paraPLAYERNAME;
	}
	if( (strcmp(para,"playernumber") == 0) || (strcmp(para,"p") == 0) || (strcmp(para,"playerNumber") == 0) || (strcmp(para,"PLAYERNUMBER") == 0)){
		return paraPLAYERNUMBER;
	}
	if( (strcmp(para,"gameVersion") == 0)){
		return paraVERSION;
	}
	return -1;
}

int assignParameters(char* ParameterLine){

	char* paraLine = ParameterLine;
	char* assParameter;
	char* assValue;
	char paraDelimiter[] = "=";
	char gamverval[256];
		if( (strcmp(paraLine," ")==0) || (strcmp(paraLine,"  ") == 0) ){
			return 0;
		}
 		//Line in Parameter und Wert aufteilen
		assParameter = strtok(paraLine,paraDelimiter); //Parameter
		//printf("assPara: \"%s\" \n", assParameter);
		paraLine = strtok(NULL,paraDelimiter); //zum wert springen

		assValue = strtok(paraLine,paraDelimiter); //Wert
		paraLine = strtok(NULL,paraDelimiter);

		//Parameter und Wert bereinigen
		assParameter = cleanBlankspace(assParameter);
		//assValue = cleanBlankspace(assValue);

		//printf("assParameter,Assvalue after cleaning:\"%s\",\"%s\" \n", assParameter, assValue);

		//Parameter erkennen und der struktur zuweisen
		switch(tellParam(assParameter)){
			case paraGAMEID:
										if(strcmp(isGameidAlreadyDefined," ") != 0){
											strcpy(confiConst.gameID, isGameidAlreadyDefined);
										}
										else{
											strcpy(confiConst.gameID, assValue);
										}
										break;
			case paraPLAYERNUMBER:
										if(strcmp(isPlayernumberAlreadyDefined," ") != 0){
											confiConst.playerNumber = atof(isPlayernumberAlreadyDefined);
										}
										else{
											confiConst.playerNumber = atof(assValue);
										}
										break;
			case paraPORT:
										confiConst.portNumber = atof(assValue);
										//strcpy(confiConst.portNumber, assValue);
										break;
			case paraPLAYERNAME:
										strcpy(confiConst.playerName, assValue);
										break;
			case paraGAMEKINDNAME:
										strcpy(confiConst.gameKindName, assValue);
										break;
			case paraHOSTNAME:
										strcpy(confiConst.hostName, assValue);
										break;
			case paraVERSION:
										strcpy(gamverval,"VERSION ");
										strcat(gamverval,assValue);
										strcat(gamverval,"\n");
										strcpy(confiConst.gameVersion, gamverval);
										break;
		 	default:
										printf("Parameter nicht erkannt: \"%s\" \n",assParameter);
										break;
		}
	return 0;
}


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
				//printf("\nLine: \"%s\"\n",configFileParameter);

				if(assignParameters(configFileParameter) == -1){
					perror("Error assigning Parameter from File to structure, READCONF");
				}
			}
		 	configFileParameter = strtok(NULL,delimiter);
		}
  }

  fclose(file);
return 0;
}
