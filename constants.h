#ifndef CONSTANTS
#define CONSTANTS

//Misc
#define ATTEMPTS_INVALID 20
#define BUFF_SIZE_SMALL 256
#define BUFFER_SIZE 256

//Configstruct
struct configConstants{
  char   gameKindName[BUFFER_SIZE];
  char   hostName[BUFFER_SIZE];
  char   gameID[BUFFER_SIZE];
  char   gameVersion[BUFFER_SIZE];
  char   playerNumber[BUFFER_SIZE];
  char   portNumber[BUFFER_SIZE]; //uint16_t portNumber;
  char   playerCount[BUFFER_SIZE];
};

struct configConstants confiConst;

//Flags
  //Rueckgabe
  #define ERROR -1
  //Prolog, perfcon und maincon
  #define MOVE 0 //vllt define success und move = 1 , wait = 2, etc. ?
  #define WAIT 1
  #define GAMEOVER 2
  #define OKTHINK 3
  #define OKWAIT 4
  #define MOVEOK 5
  #define CAPTURE 6
  #define NUMBER_OF_PLAYERS 7
  #define PLAYER_NUMBER 8
  #define PLAYER_NAME 9

//Zugberechnungen in brain.c
#define NUMBER_FIELDS 24
#define NUMBER_STONES 9
#define ZEILEN 3
#define SPALTEN 8
#define EMPTY 0
#define PLAYER_CLIENT 1
#define PLAYER_OPPONENT 2
#define A 0
#define B 1
#define C 2
#define SIZE_PLAY_COMMAND 9
#define SIZE_MOVE_COMMAND 12
#define RANDOM 0
#define NEIGHBOURING 1

//Client messages
#define MES_LENGTH_CLIENT 200
#define THINKING_MSG "THINKING\n"
#define OKWAIT_MSG "OKWAIT\n"

//Server messages
#define MES_LENGTH_SERVER 3000

//Pipe
#define PIPE_BUF 24




//Field





#endif
