#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include "shm_data.h"





int createSHM(){
int size = sizeof(struct SHM_data);
int shared_memory_id = shmget(IPC_PRIVATE, size, IPC_CREAT | IPC_EXCL);
printf("\nshared memory id is: %d\n", shared_memory_id);

//shared memory löschen wenn Thinker und Connector beendet wurden
shmctl(shared_memory_id, IPC_RMID, 0);

return shared_memory_id;
}


int writeSHM(struct SHM_data* shm_pointer, char* data, int flag){
	printf("\nStarte writeSHM\n");
	printf("\nwrite: Übergabeparameter Pointer zu: %p\n", shm_pointer);
	struct SHM_data d = *shm_pointer;
	if(flag == SPIELNAME){
		strcpy(d.spielname, data);	
		printf("\nIm struct SHM_data wurde folgender Spielname reingeschrieben: %s\n", data);
	}
return 0;
}

void readSHM(struct SHM_data* shm_pointer, int flag){
	printf("\nStarte readSHM\n");
	printf("\nread: Übergabeparameter Pointer zu: %p\n", shm_pointer);
	struct SHM_data d = *shm_pointer;
	printf("AUSLESEN spielname: %s", d.spielname);
	if(flag == SPIELNAME){
	printf("AUSLESEN spielname: %s", d.spielname);
	}

}


