#inlcude <stdio.h>
#include <string.h>
#include <stdlib.h>




int main(char filename[100]){
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
	return 0;
}
		