#ifndef SHM_DATA
#define SHM_DATA
#include "constants.h"

#define BUFFER_SIZE 256
#define SPIELNAME 0
#define FIELD 1

//Configstruct
struct configConstants{
  char   gameKindName[BUFFER_SIZE];
  char   hostName[BUFFER_SIZE];
  char   gameID[BUFFER_SIZE];
  char   gameVersion[BUFFER_SIZE];
  char   playerNumber[BUFFER_SIZE];
  char   portNumber[BUFFER_SIZE]; //uint16_t portNumber;
};

struct configConstants confiConst;

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
int field[ZEILEN][SPALTEN];
int used_pieces;
int capture_status;
int player;
};


extern int createSHM();
extern int attachSHM();
extern void writeSHM(struct SHM_data* shm_pointer, char* data, int flag);
extern void readSHM(struct SHM_data* shm_pointer);
void read_piecelist(struct SHM_data* shm_pointer, char* piecelist);

#endif
