#include "./consts.h"
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "./logs.h"
#include "./prompt.h"

#define BUFFER_SIZE 1024
#define HELP "?"

void serve_client(char *buffer, Client *client) {
    write(client->sockfd, buffer, strlen(buffer));
}

void help_client(Client *client) {
    char help_msg[] = "\n\tls\t\t\t\tlist directories and files\n\td <filename>\t\t\tdownload file\n\n";

    serve_client(help_msg, client);
}

void get_client_prompt(char *prompt, Client *client) {
    int i = 0, j = 0;
    char command[8];
    char arg[64];

    memset(command, 0, sizeof(command));
    memset(arg, 0, sizeof(arg));

    while(1) {
        command[i] = prompt[i];
        i++;

        if(prompt[i] == 0 || prompt[i] == ' ')
            break;
    }

    command[i] = 0;
    
    if(i < strlen(prompt)) {
        for(i; i < strlen(prompt); i++, j++)
            arg[j] = prompt[i];
    }

    j++;
    arg[j] = 0;

    if(strcmp(command, HELP) == 0)
        help_client(client);
}

void *handle_client(void *client_t) {
    Client *client = (Client *)client_t;
    bool is_client_connected = true;
    char buffer[BUFFER_SIZE];
    char *client_disconnected_msg[128];
    memset(client_disconnected_msg, 0, sizeof(client_disconnected_msg));
    serve_client("Connected to file server.\n? for help.\n", client);

    do {
        memset(buffer, 0, sizeof(buffer));

        int rec = recv(client->sockfd, buffer, BUFFER_SIZE, 0);
        buffer[strcspn(buffer, "\n")] = 0;
        buffer[strcspn(buffer, "\r")] = 0;

        if(rec > 0)
            get_client_prompt(buffer, client);
        else if(rec == 0) {
            printf("\b\b");
            sprintf(client_disconnected_msg, "%s disconnected.", client->addr);
            print_std_log(client_disconnected_msg);
            prompt_output("");
            is_client_connected = false;
        } else {
            printf("\b\b");
            sprintf(client_disconnected_msg, "Failed to communicate with %s. Client disconnected.", client->addr);
            print_std_log(client_disconnected_msg);
            prompt_output("");
            is_client_connected = false;
        }
    } while(is_client_connected);

    close(client->sockfd);
    pthread_detach(pthread_self());

    return NULL;
}