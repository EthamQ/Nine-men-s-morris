#ifndef CONSTANTS
#define CONSTANTS

//Main server and game information
#define GAMEKINDNAME "NMMORRIS"
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define CONFIG_DEFAULT "client.conf"



//Flags
#define ERROR -1
#define MOVE 0
#define WAIT 1
#define GAMEOVER 2


//Client messages
#define MES_LENGTH_CLIENT 200
#define WAIT_MESSAGE "OKWAIT\n"
#define MOVE_MESSAGE "THINKING\n"

//Server messages
#define MES_LENGTH_SERVER 2000

//Pipe
#define PIPE_BUF 24


//Other
#define ATTEMPTS_INVALID 20
#define BUFF_SIZE_SMALL 256


#endif