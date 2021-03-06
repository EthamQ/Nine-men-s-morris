#ifndef SHM_DATA
#define SHM_DATA
#include "constants.h"
#include <semaphore.h>
#include <sys/sem.h>

#define SPIELNAME 0
#define FIELD 1

struct spieler{
int spielernummer;
char spielername[BUFFER_SIZE];
int flag_registriert;
};


struct SHM_data{
char spielname[BUFFER_SIZE];
int anzahl_spieler;
int pid_thinker;
int pid_connector;
int field[ZEILEN][SPALTEN];
int used_pieces;
int capture_status;
int flag_think;
struct spieler client;
struct spieler opponent;
sem_t semaphore;
};


extern int createSHM();
extern int attachSHM();
extern void writeSHM(struct SHM_data* shm_pointer, char* data, int flag);
extern void readSHM(struct SHM_data* shm_pointer);
void read_piecelist(struct SHM_data* shm_pointer, char* piecelist);
void read_winner(char* piecelist, struct SHM_data* shm_pointer);

#endif
