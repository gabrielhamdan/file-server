#include "./consts.h"
#include <pthread.h>
#include <string.h>
#include <stdio.h>

void *handle_client(void *client_t) {
    Client *client = (Client *)client_t;

    write(client->sockfd, "teste", strlen("teste"));

    pthread_detach(pthread_self());

    return NULL;
}