#ifndef SERVERUTILS_H
#define SERVERUTILS_H 1

#include <stdio.h>

#define MAX_ARGC 16
#define STD_ALLOC 4
#define STD_BUF 64

#define MAX(a,b) a > b ? a : b

int debug;

int str_to_argv (char *str, char ***argv);
char *argv_to_str (char **argv);
void ndprintf (FILE *fd, char *fmt, ...);
void ndprintf_die (FILE *fd, char *fmt, ...);
void sockwrite (int fd, int status, char *msg);

#endif

