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

	if((toClean == NULL) || (strcmp(bufferChar," ") == 0) || (strcmp(bufferChar,"\0") == 0)|| (strcmp(bufferChar,"") == 0)){
		//printf("\nEinem Parameter wurde kein Wert zugewiesen, baeh\n");
		return " ";
	}
	while(bufferChar != NULL){
		toReturn = strtok(bufferChar," ");
		if((strcmp(toReturn," "))!=0){	//Alle Leerzeichen wurden entfernt
			return toReturn;
		}
		bufferChar = strtok(NULL,BlankDeli);
	}
	return " ";
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
		assValue = cleanBlankspace(assValue);

		//printf("\nassParameter,Assvalue after cleaning:\"%s\",\"%s\" \n", assParameter, assValue);

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

short checkStructurComplete(){
	/*
	printf("check\n");
	char scanVal[250];

	//gameid
	if( (strcmp(confiConst.gameID, "") == 0) || (strcmp(confiConst.gameID, " ") == 0) ){
		printf("GameID fehlt, bitte geben sie die GAMEID ein: ");
		scanf("%s",scanVal);
		strcat(confiConst.gameID,scanVal);
		printf("Neue gameID: %s\n",confiConst.gameID);
	}

	//Hostname
	if( (strcmp(confiConst.hostName, "") == 0) || (strcmp(confiConst.hostName, " ") == 0) ){
		printf("Hostname fehlt, Defaultwert nehmen? (y/n) ");
		scanf("%s",scanVal);
		if((strcmp(scanVal,"y")) || strcmp(scanVal,"yes")){ // strcmp gibt 0 zurueck wenn gleich, 0 ist gleichzeitig FALSE,
			strcat(confiConst.hostName,"sysprak.priv.lab.nm.ifi.lmu.de");
		}
		else{
			printf("Hostname eingeben: ");
			scanf("%s",scanVal);
			strcat(confiConst.hostName,scanVal);
		}
		printf("Neuer Hostname: %s\n",confiConst.hostName);
	}
	//playerNumber

	//gameKindName

	//portNumber

	//gameVersion

	//playerName


*/

	printf("\n Struktur:\n gameKindName: %s\n portNumber: %i\n hostName : %s\n gameID: %s\n playerName: %s\n playerNumber %i\n gameVersion %s\n", confiConst.gameKindName, confiConst.portNumber, confiConst.hostName,  confiConst.gameID, confiConst.playerName, confiConst.playerNumber, confiConst.gameVersion);
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

	//in line werden die einzelnen Zeilen zwischengespeichert
	char line[256];
	char* configFileParameter;
	char delimiter[] = "\n";

	while (fgets(line, sizeof(line), file)){
		configFileParameter = strtok(line,delimiter);
	 	while(configFileParameter != NULL){

			if( (strstr(configFileParameter, "§§$$")) != NULL || (strcmp(configFileParameter,"")==0) || (strcmp(configFileParameter," ")==0) ){
				//printf("Line wird ignoriert: \"%s\" \n",configFileParameter);
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
	checkStructurComplete();
  fclose(file);
return 0;
}
