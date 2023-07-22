#ifndef _CONSTS_H
#define _CONSTS_H

#include <arpa/inet.h>

#define LOCALHOST "127.0.0.1"
#define DEFAULT_PORT 30000
#define MAX_CONN_REQUEST 16

typedef struct {
	struct sockaddr_in sockaddr;
	int sockfd;
} Client;

typedef struct {
	char file_name[128];
	char file_dir[64];
	char file_alias[64];
	int file_size;
} FileData;

#endif