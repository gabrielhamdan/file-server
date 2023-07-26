#ifndef _CONSTS_H
#define _CONSTS_H

#include <arpa/inet.h>

#define LOCALHOST "127.0.0.1"
#define DEFAULT_PORT 30000
#define MAX_CONN_REQUEST 16

typedef struct {
	struct sockaddr_in sockaddr;
	int sockfd;
	char addr[64];
} Client;

#endif