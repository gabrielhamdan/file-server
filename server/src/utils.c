#include <time.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "logs.h"
#include <arpa/inet.h>

void get_client_addr(struct sockaddr_in socketaddr, char **client_address) {
    char addr[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(socketaddr.sin_addr), addr, INET_ADDRSTRLEN);

    uint16_t port = htons(socketaddr.sin_port);

	sprintf((char *)client_address, "%s:%d", addr, port);
}

char *get_timestamp() {
    time_t c_time = time(NULL);
    char *time_stamp = ctime(&c_time);
    time_stamp[strlen(time_stamp) - 1] = '\0';

    return time_stamp;
}

double timef(double t) {
    double time_elapsed = 0;
    
    if(t == 0) {
        return (double)clock();
    } else {
        clock_t stop_t = clock() - ((clock_t) t);
        time_elapsed = ((double)stop_t) / CLOCKS_PER_SEC;
    }

    return time_elapsed;
}