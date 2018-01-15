#ifndef BRAIN_H
#define BRAIN_H


char* think();
char* think_new(struct SHM_data* shm_pointer);
int check_muehle(int field[3][8]);
char* capture(struct SHM_data* shm_pointer);


#endif
