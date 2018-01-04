#ifndef CONSTANTS
#define CONSTANTS

//Main server and game information
#define GAMEKINDNAME "NMMORRIS"
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define CONFIG_DEFAULT "client.conf"



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
  //maincon
  //#define MOVETHINKING 4

//Client messages
#define MES_LENGTH_CLIENT 200
#define WAIT_MESSAGE "OKWAIT\n"
#define MOVE_MESSAGE "THINKING\n"

//Server messages
#define MES_LENGTH_SERVER 3000

//Pipe
#define PIPE_BUF 24


//Other
#define ATTEMPTS_INVALID 20
#define BUFF_SIZE_SMALL 256

//Field
#define A0 0
#define A1 1
#define A2 2
#define B0 3
#define B1 4
#define B3 5
#define C0 6
#define C1 7
#define C2 8
#define A7 9
#define B7 10
#define C7 11
#define C3 12
#define B3 13
#define A3 14
#define C6 15
#define C5 16
#define C4 17
#define B6 18
#define B5 19
#define B4 20
#define A6 21
#define A5 22
#define A4 23
int spielfeld[24];


#endif
