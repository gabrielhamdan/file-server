#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>

#define BUFF_LEN 1024

char *host = "127.0.0.1";
int port = 30000;
bool is_connected = false;

void upload(char *buff, int *fd) {
    FILE *f = fopen("./files/lorem", "r");

    if(f == NULL) {
        printf(strerror(errno));
        return;
    }

    send(*fd, buff, sizeof(buff), 0);

    char c;
    while(c = fgetc(f)) {
        send(*fd, &c, sizeof(c), 0);
        if(c == EOF)
            break;
    }

    fclose(f);
}

void *listen_t(void *fd) {
    int serverfd = (int *)fd;
    char buff[BUFF_LEN];

    while(1) {
        memset(buff, 0, sizeof(buff));

        int receive = recv(serverfd, buff, BUFF_LEN, 0);

        if(receive > 0) {
            printf("%s\n", buff);
            fflush(stdout);
        }
        else
            break;
    }

    is_connected = false;

    return NULL;
}

void *prompt_t(void *fd) {
    int serverfd = (int *)fd;
    char buff[128];

    while(1) {
        memset(buff, 0, sizeof(buff));

        fgets(buff, sizeof(buff), stdin);

        buff[strcspn(buff, "\n")] = 0;

        if(strcmp(buff, "!q") == 0)
            break;
        else if(buff[0] == 'u')
            upload(buff, &serverfd);
        else
            send(serverfd, buff, sizeof(buff), 0);
    }

    is_connected = false;

    return NULL;
}

int *client_init() {
    int serverfd;
    struct sockaddr_in server_addr;

    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(host);
    server_addr.sin_port = htons(port);

    if(connect(serverfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Couldn't connect to server.\n");
        exit(EXIT_FAILURE);
    }

    is_connected = true;

    pthread_t recv_t;
    pthread_create(&recv_t, NULL, &listen_t, (void *)serverfd);

    pthread_t send_t;
    pthread_create(&send_t, NULL, &prompt_t, (void *)serverfd);

    return &serverfd;
}

int main(int argc, char **argv) {
    int *serverfd = client_init();

    while(is_connected);

    close(&serverfd);

    return 0;
}