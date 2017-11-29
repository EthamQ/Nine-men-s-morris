#inlcude<stdio.h>
#include<string.h>
#include<stdlib.h>

struct config{
char HOSTNAME[100]
int PORTNUMBER;
char GAMEKINDNAME[50];
}


void config(char filename[100]){
	if(filename == "") {
		filename = "client.conf";
	}
	
	FILE *fp = fopen(filename,"r");
	
	if(fp == NULL) {
		printf("Fehler beim Öffnen der Datei");
	} else {
		printf("korrekt");
		char *line;
		char *readline;
		while (readline = fgets(line,256,filename)!= NULL) {
			   
			printf("eingelesener Text :%s\n",readline);
		}
	}
}
		