#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include "./logs.h"
#include <stdlib.h>
#include "./file-server.h"
#include "./client_t.h"
#include "./consts.h"

#define QUIT "!q"

bool should_shutdown = false;

void print_unknown_command(char * prompt) {
    printf("> Unknown command %s.\n", prompt);
}

void prompt_eval(char *prompt) {
    if(strlen(prompt) == 0)
        return;

    if(strcmp(prompt, QUIT) == 0)
        should_shutdown = shutdown_server();
    else
        print_unknown_command(prompt);
}

void prompt_output(char *output) {
    if(strlen(output) == 0)
        printf("> ");
    else
        printf("> %s", output);
}

void *get_prompt(void *fd) {
    int serverfd = (int *)fd;

    while(!should_shutdown) {
        char prompt[512];
        fgets(prompt, 510, stdin);
        prompt[strcspn(prompt, "\n")] = 0;

        prompt_eval(prompt);

        prompt_output("");
    }

    pthread_detach(pthread_self());

    printf("\b\b");
    print_std_log("Shutting down server.");
    close(&fd);
    exit(EXIT_SUCCESS);

    return NULL;
}