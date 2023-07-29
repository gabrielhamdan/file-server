#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include "./logs.h"
#include "./client_t.h"
#include "./consts.h"
#include <string.h>
#include <sys/stat.h>
#include <math.h>

#define FILES_PATH "./files"

double get_file_size(char *file_name) {
    struct stat file_status;

    char p_file_name[256] = FILES_PATH "/";
    strcat(p_file_name, file_name);

    if(stat(p_file_name, &file_status) < 0)
        return -1;
    
    double file_size = round(file_status.st_size) / 1000;
    
    return file_size;
}

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
        sprintf(file_name, "\t%s\t\t\t%.1gKB\n", entry->d_name, get_file_size(entry->d_name));
        serve_client(file_name, client);
    }

    if(closedir(dir) == -1)
        print_err_log(strerror(errno));
}

void upload_file(char *file_name,  Client *client) {
    char p_file_name[256] = FILES_PATH "/";
    strcat(p_file_name, file_name);

    FILE *f = fopen(p_file_name, "w");

    if(f == NULL)
        print_err_log(strerror(errno));

    if(f != NULL) {
        char c;
        while(1) {
            recv(client->sockfd, &c, sizeof(char), 0);
            if(c == EOF)
                break;

            fputc(c, f);
        }

        if(fclose(f) != 0)
            print_err_log(strerror(errno));
    }

    print_std_log("File was successfully uploaded.");
}

void download_file(char *file_name, Client *client) {
    char p_file_name[256] = FILES_PATH "/";
    strcat(p_file_name, file_name);

    FILE *f = fopen(p_file_name, "r");

    if(f == NULL) {
        if(errno == ENOENT)
            serve_client(strerror(errno), client);
        else
            print_err_log(strerror(errno));
    } else {
        char buffer[1024];
        size_t bytesRead;

        while ((bytesRead = fread(buffer, sizeof(char), 1024, f)) > 0) {
            send(client->sockfd, buffer, bytesRead, 0);
        }
        
        if(fclose(f) != 0)
            print_err_log(strerror(errno));
    }
}