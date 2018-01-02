#ifndef SHM_DATA
#define SHM_DATA
#define BUFFER_SIZE 256
#define SPIELNAME 0 

struct spieler{
char spielernummer[BUFFER_SIZE];
char name[BUFFER_SIZE];
int flag_registriert;
};


struct SHM_data{
char spielname[BUFFER_SIZE];
char spielernummer[BUFFER_SIZE];
char anzahl_spieler[BUFFER_SIZE];
int pid_thinker;
int pid_connector;
};


extern int createSHM();
extern int attachSHM();
extern void writeSHM(struct SHM_data* shm_pointer, char* data, int flag);
extern void readSHM(struct SHM_data* shm_pointer);

#endif
