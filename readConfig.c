#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config_header.h"


int read_configfile(char* configname){
FILE* file;
char* filepath = configname;

file = fopen(filepath, "r");
if(file == NULL){
perror("No such config file");
return -1;
}
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
