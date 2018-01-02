#ifndef maintainConnection_H
#define maintainConnection_H

int maintainConnection(int sockfd);
int maintainConnectionFIRST(int sockfd, int firstServerCommand);

short conWAIT(int sockfd);

short conGAMEOVER(int sockfd);

short conMOVE(int sockfd);

short conPlay(int sockfd, char* move);
#endif
