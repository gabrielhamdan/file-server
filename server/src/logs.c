#include "utils.h"
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "./prompt.h"

#define LOGS_DIR "./logs"
#define STD_LOG_P "/std_log.txt"
#define ERR_LOG_P "/err_log.txt"
#define RED_COL "\033[0;31m"
#define DEFAULT_COL "\033[0m"

void logs_init() {
    int r_mkdir = mkdir(LOGS_DIR, S_IRWXU);

    if(r_mkdir < 0)
        print_log(strerror(errno), true, "");
    else
        print_log("Logs directory was successfully created.", false, "");
}

bool f_log(const char *log, const char *path) {
    if(strlen(path) <= 0)
        return true;

    FILE *std_log_file;
    std_log_file = fopen(path, "a");

    if(std_log_file == NULL)
        return false;

    fprintf(std_log_file, "%s", log);
    fclose(std_log_file);

    return true;
}

void print_log(const char *log, bool is_err, const char *path) {
    char *line;
    char *timestamp = get_timestamp();
    line = malloc((strlen(timestamp) + strlen(log) + 16) * sizeof(char));
    sprintf(line, "[%s] %s\n", timestamp, log);

    if(is_err)
        printf(RED_COL);
    else
        printf(DEFAULT_COL);

    printf("%s", line);

    bool f_logged = f_log(line, path);
    if(!f_logged) {
        printf(RED_COL);
        printf("Failed to write to %s\n", path);
    }

    free(line);
    line = NULL;
}

void print_std_log(const char *log) {
    print_log(log, false, LOGS_DIR STD_LOG_P);
}

void print_err_log(const char *err) {
    print_log(err, true, LOGS_DIR ERR_LOG_P);
}