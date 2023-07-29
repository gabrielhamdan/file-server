#ifndef _S_FILES_H
#define _S_FILES_H

#include "./consts.h"

void list_dir(Client *);
void download_file(char *, Client *);
void upload_file(char *,  Client *);

#endif