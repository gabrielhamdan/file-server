#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include "./logs.h"
#include "./client_t.h"
#include "./consts.h"
#include <string.h>

#define FILES_PATH "./files"

void list_dir(Client *client) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(FILES_PATH);

    if(dir == NULL) {
        print_err_log(strerror(errno));
        return;
    }

    while((entry = readdir(dir)) != NULL) {
        if(entry->d_name[0] == '.')
            continue;

        char file_name[128];
        sprintf(file_name, "%s\n", entry->d_name);
        serve_client(file_name, client);
    }

    if(closedir(dir) == -1)
        print_err_log(strerror(errno));
}