#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include "shm_data.h"





int createSHM(){
  int shmid;
	if((shmid =shmget(IPC_PRIVATE,  sizeof(struct SHM_data), IPC_CREAT | 0666) < 0)){
	perror("\nFehler bei Erstellung der shared memory\n");
	return -1;
	}
	else{
	printf("\nshared memory success\n");
	printf("\nshared memory id is: %d\n", shmid);
	//shared memory löschen wenn Thinker und Connector beendet wurden
	//shmctl(shmid, IPC_RMID, 0);
	}
	shmat(shmid, NULL, 0);
	
	return shmid;
}


int writeSHM(struct SHM_data* shm_pointer, char* data, int flag){
	printf("\nStarte writeSHM\n");
	printf("\nwrite: Übergabeparameter Pointer zu: %p\n", shm_pointer);
	//struct SHM_data d = *shm_pointer;
	if(flag == SPIELNAME){
		strcpy(shm_pointer->spielname, data);	
		printf("\nwrite: Im struct SHM_data wurde folgender Spielname reingeschrieben: %s\n", data);
		printf("write: AUSLESEN spielname: %s", shm_pointer->spielname);
	}
return 0;
}

void readSHM(struct SHM_data* shm_pointer, int flag){
	printf("\nStarte readSHM\n");
	printf("\nread: Übergabeparameter Pointer zu: %p\n", shm_pointer);
	printf("Flag: %i", flag);
	printf("read: AUSLESEN spielname: %s", shm_pointer->spielname);
	
	
	

}


