#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define BUFFER_SIZE 256
//move structs in a header file later

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
struct spieler s;
};



int createSHM(){
struct shm_data data;
int shared_memory_id = shmget(IPC_PRIVATE, sizeof(data), IPC_CREAT | IPC_EXCL);
printf("shared memory id is: %d\n", shared_memory_id);
int *ptr = shmat(shared_memory_id, 0, 0);
printf("shmat returns a pointer to: %p\n", ptr);
if(*ptr==-1){
perror("Fehler bei smat");
}
return shared_memory_id;
}

int writeSHM(){

}

int readSHM(){

}

int main(){
createSHM();
}
