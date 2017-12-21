#ifndef maintainConnection_H
#define maintainConnection_H

short maintainConnection(int sockfd, char* firstServerList);

short conWAIT(int sockfd);

short conGAMEOVER(int sockfd);

short conMOVE(int sockfd);

#endif
