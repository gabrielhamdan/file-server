#ifndef _CLIENT_T_H
#define _CLIENT_T_H

#include "./consts.h"

void *handle_client(void *);
void help_client(Client *);
void serve_client(char *, Client *);

#endif