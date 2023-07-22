#ifndef _LOGS_H
#define _LOGS_H

/**
 * Creates logs directory.
*/
void logs_init();
/**
 * Writes standard log to stdout and to stream.
*/
void print_std_log(const char *);
/**
 * Writes errors to stdout and to stream.
*/
void print_err_log(const char *);

#endif