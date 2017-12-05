#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm_data.h"
#define BUFFER_SIZE 256



int createSHM(){
struct shm_data data;
int shared_memory_id = shmget(IPC_PRIVATE, sizeof(data), IPC_CREAT | IPC_EXCL);
printf("shared memory id is: %d\n", shared_memory_id);

//shared memory löschen wenn Thinker und Connector beendet wurden
shmctl(shared_memory_id, IPC_RMID, 0);

return shared_memory_id;
}

int attachSHM(){
int *ptr = shmat(shared_memory_id, 0, 0);
printf("shmat returns a pointer to: %p\n", ptr);
if(*ptr==-1){
perror("Fehler bei smat");
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
