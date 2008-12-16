#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "net_utils.h"

#define STD_CLIENTS 5

int max_clients = 0;
extern int debug;

void
usage (char *prog, int retval)
{
	printf("Usage %s -n <clients> [-d]\n"
		"\t-n <clients>\tNumber of clients to start the game (has to be bigger than 0)\n"
		"\t-d\tEnables debug mode\n", prog);
	exit(retval);
}

int 
main (int argc, char *argv[])
{
	int retval;

	while ((retval = getopt(argc, argv, "dn:")) != -1) {
		switch (retval) {
			case 'd':
				debug = 1;
				break;
			case 'n':
				max_clients = atoi(optarg);
				break;
			case 'h':
				usage(argv[0], EXIT_SUCCESS);
				break;
			default:
				break;
		}
	}

	if (argc > optind)
		usage(argv[0], EXIT_FAILURE);

	if (max_clients <= 0)
		max_clients = STD_CLIENTS;

	if (!debug) {
		switch (fork()) {
			case -1:
				fprintf(stderr, "fork(): %s\n", strerror(errno));
				exit(EXIT_FAILURE);
			case 0:
				break;
			default:
				exit(EXIT_SUCCESS);
		}
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
	}
	ndprintf(stdout, "[SERVER] Starting server...\n");
	return 0;
}
