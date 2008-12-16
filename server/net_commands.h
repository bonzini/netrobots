#ifndef NET_COMMANDS_H
#define NET_COMMANDS_H 1

#include <stdbool.h>

typedef struct cmd_t {
        char *command;
        void (*func) (int, int);
	int args;
	bool need_cycle;
} cmd_t;


#endif
