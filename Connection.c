#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <netdb.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#define BUF 256
#define Game-ID "NMMORRIS"
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define BUF_SIZE 256
#define MES_LENGTH_SERVER 100
#define ATTEMPTS_INVALID 20
static char dataPRS[MES_LENGTH_SERVER];
static char versionPRC []= "Version 2.0";
static char game_idPRC []= "ID <<Game-ID>>";
static char  numberOfPlayersPRC []= "Player";

//ARGS: server message, if it begins with "+" return true
bool static serverResponseValid(const char r[]){
    if(strcmp(r[0], "+") == 0){
        return true;
    }
    return false;
}




int static performConnection(int sockfd){
    if(sockfd < 0){
    perror("Invalid socket file descriptor");
        close(sockfd);
        return -1;
    }else{
        printf("Good to go?!");
        }
    //if testifvalid is negative then there was an error, repeat write() or read() if error
ssize_t testifvalid = -1;
//Number of invalid attempts you're allowed to have, amount -> see #define ATTEMPTS_INVALID, counts attempts up to this number
int attempts = 0;

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
        testifvalid = -1;

//C: <<Client Version>>
    while(testifvalid < 0){
        testifvalid = write(sockfd, versionPRC, (int)strlen(versionPRC));
            attempts++;
            if(attempts >= ATTEMPTS_INVALID){
                return -1;
            }
    }
    testifvalid = -1;

//S: Game-ID request
    while(testifvalid < 0){
        testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
            if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
                perror("Invalid server respone");
                return -1;
        }
    }
    testifvalid = -1;

    //C: <<Game-ID>>
    while(testifvalid < 0){
        testifvalid = write(sockfd, game_idPRC, (int)strlen(game_idPRC));
        attempts++;
        if(attempts >= ATTEMPTS_INVALID){
            return -1;
        }
    }
    testifvalid = -1;

//S: <<Gamekind-Name>>
    while(testifvalid < 0){
        testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
        if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
            perror("Invalid server respone");
            return -1;
        }
    }
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

//C: [[Gew�nschte Mitspielernummer]]
    while(testifvalid < 0){
        testifvalid = write(sockfd, numberOfPlayersPRC, (int)strlen(numberOfPlayersPRC));
        attempts++;
        if(attempts >= ATTEMPTS_INVALID){
            return -1;
        }
    }
    testifvalid = -1;

//S: <<Mitspielernummer>> <<Mitspielername>>
    while(testifvalid < 0){
    testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
        if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
        perror("Invalid server respone");
    return -1;
    }
    }
    testifvalid = -1;

//S: <<Mitspieleranzahl>>
    while(testifvalid < 0){
        testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
        if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
        perror("Invalid server respone");
        return -1;
    }
    }
    testifvalid = -1;

//S: <<Mitspielernummer>> <<Mitspielername>> <<Bereit>>
    while(testifvalid < 0){
        testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);

        if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
            perror("Invalid server response");
            return -1;
        }
    }
    testifvalid = -1;
    return 0;
}




