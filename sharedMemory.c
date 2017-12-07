#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm_data.h"
#define BUFFER_SIZE 256

struct spieler{
char spielernummer[BUFFER_SIZE];
char name[BUFFER_SIZE];
int flag_registriert;
};


struct shm_data{
char spielname[BUFFER_SIZE];
char spielernummer[BUFFER_SIZE];
char anzahl_spieler[BUFFER_SIZE];
int pid_thinker;
int pid_connector;
};



int createSHM(){
int size = sizeof(struct shm_data);
int shared_memory_id = shmget(IPC_PRIVATE, size, IPC_CREAT | IPC_EXCL);
printf("shared memory id is: %d\n", shared_memory_id);

//shared memory löschen wenn Thinker und Connector beendet wurden
shmctl(shared_memory_id, IPC_RMID, 0);

return shared_memory_id;
}

int attachSHM(int shmID){
int *ptr = shmat(shmID, 0, 0);
printf("shmat returns a pointer to: %p\n", ptr);
if(*ptr==-1){
perror("Fehler bei smat");
return 0;
}

}

int writeSHM(){
return 0;
}

int readSHM(){
return 0;
}

int main(){
createSHM();
}
