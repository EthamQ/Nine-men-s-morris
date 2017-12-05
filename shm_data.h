#ifndef shm_data
#define shm_data
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
struct spieler s;
};

int createSHM();

#endif
