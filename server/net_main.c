#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "net_utils.h"
#include "net_defines.h"
#include "robotserver.h"

#define STD_CLIENTS 5
#define STD_HOSTNAME "127.0.0.1"
#define STD_PORT "4300"

extern int debug;

void
usage (char *prog, int retval)
{
	printf("Usage %s [-n <clients> -H <hostname> -P <port> -d]\n"
		"\t-n <clients>\tNumber of clients to start the game (has to be bigger than 0) (Default: 5)\n"
		"\t-H <hostname>\tSpecifies hostname (Default: 127.0.0.1)\n"
		"\t-P <port>\tSpecifies port (Default: 4300)\n"
		"\t-d\tEnables debug mode\n", prog);
	exit(retval);
}

int 
main (int argc, char *argv[])
{
	int retval;

	char *port = STD_PORT, *hostname = STD_HOSTNAME;

	while ((retval = getopt(argc, argv, "dn:hH:P:")) != -1) {
		switch (retval) {
			case 'H':
				hostname = optarg;
				break;
			case 'P':
				port = optarg;
				break;
			case 'd':
				debug = 1;
				break;
			case 'n':
				max_robots = atoi(optarg);
				break;
			case 'h':
				usage(argv[0], EXIT_SUCCESS);
				break;
			default:
				break;
		}
	}

	if (argc > optind || !hostname || !port)
		usage(argv[0], EXIT_FAILURE);

	if (max_robots <= 0)
		max_robots = STD_CLIENTS;
	server_init(hostname, port);
	return 0;
}
