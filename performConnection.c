#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#define MESG_LENGTH_SERVER 100
#define ATTEMPTS_INVALID 20

//ARGS: server message, if it begins with "+" return true
bool serverResponseValid(*char r){
if(strcmp(r[0], "+") == 0){
return true;
}
return false;
}

//implements the communication with the server (Prolog)
//returns -1 if error, else 0 (later socket filedecriptor)
static int performConnection(int sockfd, int gameid){
//Abkürzungen: PR=Prolog, S=Server, C=Client

//store the messages of the Server (later with read())
static char dataPRS[MES_LENGTH_SERVER];

//Messages the Client sends to the server (later with write())
static char[] versionPRC = "Version 2.0";
//game id to string
static char buffer[256]
itoa (gameid,buffer,256);
static char[] game_idPRC = strcat("ID ", buffer);

static char[] player_number_PRC = "Player";

//invalid socket
if(fdSocket < 0){
perror("Invalid socket file descriptor");
close(sockfd);
return -1;
}

//if testifvalid is negative then there was an error, repeat write() or read() if error
static ssize_t testifvalid = -1;
//Number of invalid attempts you're allowed to have, amount -> see #define ATTEMPTS_INVALID, counts attempts up to this number
static int attempts = 0;

//TODO: if Server doesn't respond with "+" -> error handling
//S: <<Gameserver Version>>
while(testifvalid < 0){
testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
attempts++;
if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
perror("Invalid server response");
return -1;
}
}
printf("S: %s\n", dataPRS);
testifvalid = -1;

//C: <<Client Version>>
while(testifvalid < 0){
testifvalid = write(sockfd, versionPR, (int)strlen(versionPR));
attempts++;
if(attempts >= ATTEMPTS_INVALID){
return -1;
}
}
printf("C: %s\n", versionPR);
testifvalid = -1;

//S: Game-ID request
while(testifvalid < 0){
testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
perror("Invalid server respone");
return -1;
}
}
printf("S: %s\n", dataPRS);
testifvalid = -1;

//C: <<Game-ID>>
while(testifvalid < 0){
testifvalid = write(sockfd, game_idPRC, (int)strlen(game_idPRC));
attempts++;
if(attempts >= ATTEMPTS_INVALID){
return -1;
}
}
printf("C: %s\n", gameidPRC);
testifvalid = -1;

//S: <<Gamekind-Name>>
while(testifvalid < 0){
testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
perror("Invalid server respone");
return -1;
}
if(strcmp(dataPRS,"NMMorris") != 0){
printf("Ungültige Spielart!");
return -1;
}
}
printf("S: %s\n", dataPRS);
testifvalid = -1;

//S: <<Game-Name>>
while(testifvalid < 0){
testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
perror("Invalid server respone");
return -1;
}
}
testifvalid = -1;

//C: [[Gewünschte Mitspielernummer]]
while(testifvalid < 0){
testifvalid = write(sockfd, player_number_PRC, (int)strlen(numberOfPlayersPRC));
attempts++;
if(attempts >= ATTEMPTS_INVALID){
return -1;
}
}
printf("C: %s\n", player_number_PRC);
testifvalid = -1;

//S: <<Mitspielernummer>> <<Mitspielername>>
while(testifvalid < 0){
testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
perror("Invalid server respone");
return -1;
}
}
printf("S: %s\n", dataPRS);
testifvalid = -1;

//S: <<Mitspieleranzahl>>
while(testifvalid < 0){
testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
perror("Invalid server respone");
return -1;
}
}
printf("S: %s\n", dataPRS);
testifvalid = -1;

//S: <<Mitspielernummer>> <<Mitspielername>> <<Bereit>>
while(testifvalid < 0){
testifvalid = read(sockfd, dataPRFS, MES_LENGTH_SERVER){
if(!serverResponseValid(dataRPS) || attempts >= ATTEMPTS_INVALID){
perror("Invalid server response");
return -1;
}
}
printf("S: %s\n", dataPRS);
testifvalid = -1;


//S: + ENDPLAYERS
while(testifvalid < 0){
testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
perror("Invalid server respone");
return -1;
}
}
printf("S: %s\n", dataPRS);
testifvalid = -1;


return sockfd;
}


