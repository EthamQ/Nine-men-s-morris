#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>


int main(){

pid_t pid;
switch(pid = fork()){
case -1: perror("Fehler bei fork\n"); 
return -1;
break;
case 0: printf("Kindprozess(Connector) mit der id %d und der Variable pid = %d. Mein Elternprozess ist: %d\n", getpid(), pid, getppid());
//Code for Connector (probably call of main.c)
exit(0);
break;
default: printf("Elternprozess(Thinker) mit der id %d und der Variable pid = %d. MeinElternprozess ist: %d\n", getpid(), pid, getppid());
//Code for Thinker
wait(NULL);
break;
}

return 0;
}
