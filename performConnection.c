#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#define MESG_LENGTH_SERVER 100
int main(){

}

//implements the communication with the server (Prolog)
void performConnection(int sockfd){
//Abkürzungen: PR=Prolog, S=Server, C=Client

//store the messages of the Server (later with read())
char dataPRS[MES_LENGTH_SERVER];

//Messages the Client sends to the server (later with write())
//TODO: add id, number of players (as method argument?)
char[] versionPRC = "Version 2.0";
char[] game_idPRC = "ID <<Game-ID>>"; 
char[] numberOfPlayersPRC = "Player [[Gewünschte Spielernummer]]";

//invalid socket
if(fdSocket < 0){
perror("Invalid socket file descriptor");
close(sockfd);
}

//if testifvalid is negative then there was an error, repeat write() or read() if error
ssize_t testifvalid = -1;

//TODO: if Server doesn't respond with "+" -> error handling
//TODO: end while loops after a certain amount of attempts
//S: <<Gameserver Version>>
while(testifvalid < 0){
testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
if(!serverResponseValid(dataPRS)){
perror("Invalid server respone");
}
}
testifvalid = -1;

//C: <<Client Version>>
while(testifvalid < 0){
testifvalid = write(sockfd, versionPR, (int)strlen(versionPR));
}
testifvalid = -1;

//S: Game-ID request
while(testifvalid < 0){
testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
if(!serverResponseValid(dataPRS)){
perror("Invalid server respone");
}

}
testifvalid = -1;

//C: <<Game-ID>>
while(testifvalid < 0){
testifvalid = write(sockfd, game_idPRC, (int)strlen(game_idPRC));
}
testifvalid = -1;

//S: <<Gamekind-Name>>
while(testifvalid < 0){
testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
if(!serverResponseValid(dataPRS)){
perror("Invalid server respone");
}

}
testifvalid = -1;

//S: <<Game-Name>>
while(testifvalid < 0){
testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
if(!serverResponseValid(dataPRS)){
perror("Invalid server respone");
}

}
testifvalid = -1;

//C: [[Gewünschte Mitspielernummer]]
while(testifvalid < 0){
testifvalid = write(sockfd, numberOfPlayersPRC, (int)strlen(numberOfPlayersPRC));
}
testifvalid = -1;

//S: <<Mitspielernummer>> <<Mitspielername>>
while(testifvalid < 0){
testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
if(!serverResponseValid(dataPRS)){
perror("Invalid server respone");
}
}
testifvalid = -1;

//S: <<Mitspieleranzahl>>
while(testifvalid < 0){
testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
if(!serverResponseValid(dataPRS)){
perror("Invalid server respone");
}
}
testifvalid = -1;

}

//ARGS: server message, if it begins with "+" return true
bool serverResponseValid(*char r){
if(strcmp(r[0], "+") == 0){
return true;
}
return false;
}
