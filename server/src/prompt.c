#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include "./logs.h"
#include <stdlib.h>
#include "./file-server.h"

#define QUIT "!q"

bool should_shutdown = false;

void prompt_eval(char *prompt) {
    if(strlen(prompt) == 0)
        return;

    if(strcmp(prompt, QUIT) == 0)
        should_shutdown = shutdown_server();
    else
        printf("> Unknown command %s.\n", prompt);
}

void prompt_output(char *output) {
    if(strlen(output) == 0)
        printf("> ");
    else
        printf("> %s", output);
}

void *get_prompt() {

    while(!should_shutdown) {
        fflush(stdout);
        prompt_output("");

        char prompt[512];
        fgets(prompt, 510, stdin);
        prompt[strcspn(prompt, "\n")] = 0;

        prompt_eval(prompt);
    }

    pthread_detach(pthread_self());

    print_std_log("Shutting down server.");
    exit(EXIT_SUCCESS);

    return NULL;
}