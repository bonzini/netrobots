#ifndef NET_DEFINES_H
#define NET_DEFINES_H 1

#include <stdbool.h>

typedef struct cmd_t {
	void (*func) (int, int);
	int args;
	bool cycle;
} cmd_t;

#define ERROR 0
#define OK 1
#define START 2
#define END 3

void init_server(char *hostname, char *port);

#endif
