#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"


int read_configfile(){
FILE* file;
//zu Testzwecken
//char* filepath = "/mnt/c/Users/rapha/Desktop/config.txt";
char* filepath = "/root/SysPrak/MuehleGruppe13/config.txt";
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
