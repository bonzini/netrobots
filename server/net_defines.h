#ifndef NET_DEFINES_H
#define NET_DEFINES_H 1

#include <stdbool.h>

#include "robotserver.h"
#include "net_command_list.h"

typedef struct cmd_t {
	int (*func) (struct robot *robot, int *args);
	int args;
	bool cycle;
} cmd_t;

typedef struct result_t {
	int result;
	bool error;
	bool cycle;
} result_t;


void init_server(char *hostname, char *port);
result_t execute_cmd (struct robot *robot, char *input);

#endif
