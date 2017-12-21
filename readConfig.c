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
while (fgets(line, sizeof(line), file)) {
        printf("%s", line); 
    }
	printf("\n");

    fclose(file);

return 0;
}

//int main(){
//read_configfile();
//return 0;
//}
