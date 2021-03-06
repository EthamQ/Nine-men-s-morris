#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <netdb.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "constants.h"
#include "shm_data.h"

int maintainConnection(int sockfd, struct SHM_data* shm_pointer){
	char *serverResponse=malloc(sizeof(char)*MES_LENGTH_SERVER);
	memset(serverResponse, 0, MES_LENGTH_SERVER);

	//Servernachricht auslesen
	if((read(sockfd, serverResponse, sizeof(char)*MES_LENGTH_SERVER))<0){
	  perror("\nmaintainConnection(): read error, MAINCON\n");
		free(serverResponse);
		return ERROR;
	}

	//Inhalt Servernachricht überprüfen
	if(strstr(serverResponse,"+ MOVE ")){
	//send THINKING
	//printf("\nmaintainConnection(): received +MOVE from the server\n");
	if(write(sockfd, THINKING_MSG, (int)strlen(THINKING_MSG)) <0){
		perror("Fehler beim Senden von THINKING");
	}
	//printf("C: %s", THINKING_MSG);

	//receive OKTHINK
	if(read(sockfd, serverResponse, sizeof(char)*MES_LENGTH_SERVER) < 0){
		perror("Fehler beim Empfangen von OKTHINK");
	}
		//printf("\nS: %s",serverResponse);
		read_piecelist(shm_pointer, serverResponse);
		free(serverResponse);
		return MOVE;
  }

	if(strstr(serverResponse,"+ WAIT")){
			//printf("\nmaintainConnection(): received +WAIT from the server\n");
			//send OKWAIT
			write(sockfd, OKWAIT_MSG, (int)strlen(OKWAIT_MSG));
			//printf("C: %s", OKWAIT_MSG);
			printf("Der Gegner ist am Zug\n");
			free(serverResponse);
			return WAIT;
    }

	if(strstr(serverResponse,"+ MOVEOK")){
			//printf("maintainConnection(): received + MOVEOK from the server\n");			
			free(serverResponse);
			return MOVEOK;
    }

	if(strstr(serverResponse,"OKTHINK")){
			//printf("\nmaintainConnection(): received +OKTHINK from the server\n");
			free(serverResponse);
			return OKTHINK;
    }

	if(strstr(serverResponse,"CAPTURE") && strstr(serverResponse,"+ MOVE")){
			free(serverResponse);
			return CAPTURE;
	}

	if(strstr(serverResponse,"+ GAMEOVER")){
		//printf("\nmaintainConnection(): received +GAMEOVER from the server\n");
		read_winner(serverResponse, shm_pointer);
		free(serverResponse);
		return GAMEOVER;
	}
	printf("client ist Spieler %i\n",shm_pointer->client.spielernummer);
	printf("Fehler bei Servermessage:%s\n",serverResponse);
	free(serverResponse);
	return ERROR;
}


short send_move_to_server(int sockfd, char* move){
	if(write(sockfd, move, strlen(move)) < 0){
		perror("write error, MAINCON");
		return ERROR;
	}
	//printf("\nClient macht folgenden Zug %s\n", move);
	//printf("length of this command: %zu", strlen(move));
	return 0;
}
