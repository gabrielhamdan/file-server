#ifndef _UTILS_H
#define _UTILS_H

#include <stdbool.h>

/**
 * @brief Return formatted timestamp.
 */
char *get_timestamp();
/**
 * @brief Time execution between a code block.
 * @param t 
 * 
 * @return CPU time between @code{.c} double t = timef(0, sould_print_log) and timef(t, sould_print_log) @endcode .
*/
double timef(double);

#endif