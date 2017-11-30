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
#include"performConnection.h"
#define BUF 256
#define GAMEKINDNAME "NMMORRIS"
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define BUF_SIZE 256
#define MES_LENGTH_SERVER 100
#define ATTEMPTS_INVALID 20
static char dataPRS[MES_LENGTH_SERVER];
static char versionPRC []= "VERSION 2.0\n";
static char game_idPRC []= "ID 1234567891234\n";
static char  numberOfPlayersPRC []= "2\n";

//ARGS: server message, if it begins with "+" return true
static bool serverResponseValid(const char r[]){
    if(strncmp(r, "+",1) == 0){
        return true;
    }
    return false;
}




 int performConnection(int sockfd){

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
                perror("Invalid server response2");
                return -1;
            }

        }
        printf("%s\n",dataPRS);
        testifvalid = -1;

//C: <<Client Version>>
    while(testifvalid < 0){
        testifvalid = write(sockfd, versionPRC, (int)strlen(versionPRC));
            attempts++;
            if(attempts >= ATTEMPTS_INVALID){
                return -1;
            }
    }
    printf("%s\n",versionPRC);
    testifvalid = -1;

//S: Game-ID request
    while(testifvalid < 0){
        testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
            if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
                perror("Invalid server response3");
                printf("%s\n",dataPRS);
                return -1;
        }
    }
    printf("%s\n",dataPRS);
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
            perror("Invalid server response4");
            return -1;
        }
    }
    printf("%s\n",dataPRS);
    testifvalid = -1;

        //S: <<Game-Name>>
    while(testifvalid < 0){
        testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
        if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
            perror("Invalid server response");
            return -1;
        }
    }
    printf("%s\n",dataPRS);
    testifvalid = -1;

//C: [[GewŸnschte Mitspielernummer]]
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
        perror("Invalid server response5");
    return -1;
    }
    }
    printf("%s\n",dataPRS);
    testifvalid = -1;

//S: <<Mitspieleranzahl>>
    while(testifvalid < 0){
        testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);
        if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
        perror("Invalid server response6");
        return -1;
    }
    }
    printf("%s\n",dataPRS);
    testifvalid = -1;

//S: <<Mitspielernummer>> <<Mitspielername>> <<Bereit>>
    while(testifvalid < 0){
        testifvalid = read(sockfd, dataPRS, MES_LENGTH_SERVER);

        if(!serverResponseValid(dataPRS) || attempts >= ATTEMPTS_INVALID){
            perror("Invalid server response7");
            return -1;
        }
    }
    printf("%s\n",dataPRS);
    testifvalid = -1;
    

return sockfd;
}


