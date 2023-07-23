#include "./consts.h"
#include "utils.h"
#include "./logs.h"
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include "./client_t.h"
#include "./prompt.h"
#include <stdio.h>
#include "./message.h"

char *host = LOCALHOST;
int port = DEFAULT_PORT;

void server_init() {
    double t = timef(0);

    char *server_init_msg;
    server_init_msg = malloc((strlen(host) + sizeof(int) + 32) * sizeof(char));
    sprintf(server_init_msg, "Initializing server at %s:%d.", host, port);
    print_std_log(server_init_msg);
    free(server_init_msg);
    server_init_msg = NULL;

    int serverfd = 0;
    int *p_serverfd = &serverfd;
    struct sockaddr_in serv_addr;

    if((serverfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        print_err_log(strerror(errno));
        exit(EXIT_FAILURE);
    } else print_std_log("Socket was successfully created.");

    if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) {
        print_err_log(strerror(errno));
        exit(EXIT_FAILURE);
    }

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
	inet_pton(AF_INET, host, &(serv_addr.sin_addr));
	serv_addr.sin_port = htons(port);

    if(bind(serverfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		print_err_log(strerror(errno));
        exit(EXIT_FAILURE);
	} else print_std_log("Socket was successfully bound.");

    if (listen(serverfd, MAX_CONN_REQUEST) < 0) {
		print_err_log(strerror(errno));
        exit(EXIT_FAILURE);
	} else {
        char *listening_msg;
        listening_msg = malloc((sizeof(port) + 32) * sizeof(char));
        sprintf(listening_msg, "Listening on port %d.", port);
        print_std_log(listening_msg);
        free(listening_msg);
        listening_msg = NULL;
    }

    double time_elapsed = timef(t);
    char *startup_message;
    startup_message = malloc(32 * sizeof(char) + sizeof(time_elapsed));
    sprintf(startup_message, "Server startup in %lf seconds.", time_elapsed);
    print_std_log(startup_message);
    free(startup_message);
    startup_message = NULL;

    pthread_t t_id;
    pthread_create(&t_id, NULL, &get_prompt, (void *)serverfd);

    keep_listening(&serverfd);
}

void keep_listening(int *serverfd) {
    pthread_t t_id;
    char s_client_addr[64];
    char accept_msg[128];

    while(1){
        print_std_log("Waiting for a new connection.");
        prompt_output("");

        struct sockaddr_in cli_addr;
        socklen_t clienteLen = sizeof(cli_addr);
        int connfd = accept(*serverfd, (struct sockaddr*)&cli_addr, &clienteLen);

        Client *client = (Client *)malloc(sizeof(Client));
        client->sockaddr = cli_addr;
        client->sockfd = connfd;
        get_client_addr(client->sockaddr, client->addr);

        printf("\b\b");
        memset(accept_msg, 0 , sizeof(accept_msg));
        sprintf(accept_msg, "%s %s", client->addr, CLIENT_ACCEPTED_MSG);
        print_std_log(accept_msg);

        pthread_create(&t_id, NULL, &handle_client, (void*)client);
    }
}

bool shutdown_server() {
    prompt_output("Shut down server? (y/n) ");
    char prompt[8];
    fgets(prompt, 8, stdin);
    prompt[strcspn(prompt, "\n")] = 0;

    if(strcmp(prompt, "y") == 0 || strcmp(prompt, "Y") == 0)
        return true;
    else
        print_unknown_command(prompt);
    
    return false;
}

int main(int argc, char **argv) {
    setvbuf(stdout, NULL, _IONBF, 0);

    if(argc >= 2) {
        host = malloc(strlen(argv[1]) * sizeof(char));
        strcpy(host, argv[1]);
    }

    if(argc == 3)
        port = atoi(argv[2]);

    logs_init();
    server_init();

    if(argc >= 2)
        free(host);

    return EXIT_SUCCESS;
}