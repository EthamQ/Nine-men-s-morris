#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int read_configfile(char* gameID, char* playerNumber, char* configFileName){
FILE* file;
char* filepath = configFileName;

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
