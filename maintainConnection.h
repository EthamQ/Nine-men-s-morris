#ifndef maintainConnection_H
#define maintainConnection_H

int maintainConnection(int sockfd, struct SHM_data* shm_pointer);
int maintainConnectionFIRST(int sockfd, int firstServerCommand);

short send_move_to_server(int sockfd, char* move);
#endif
