#ifndef maintainConnection_H
#define maintainConnection_H

int maintainConnection(int sockfd);
int maintainConnectionFIRST(int sockfd, int firstServerCommand);

short conWAIT(int sockfd);

short conGAMEOVER(int sockfd);

short conMOVE(int sockfd);

int testplay(int sockfd);

short send_move_to_server(int sockfd, char* move);
#endif
