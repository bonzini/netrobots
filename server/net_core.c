#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

#include "net_utils.h"
#include "net_defines.h"
#include "robotserver.h"
#include "drawing.h"

#define STD_CLIENTS 5
#define STD_CYCLES 10000
#define STD_HOSTNAME NULL
#define STD_PORT "4300"

int current_robots = 0;
struct pollfd *fds;

extern int debug;
extern int max_robots;

int max_cycles;
int current_cycles = 0;

struct robot **all_robots;

int
create_client (int fd)
{
	struct pollfd pollfd;

	if (fd == -1)
		return 0;
	struct robot *r;
	if (!(r = (struct robot *) malloc (sizeof(struct robot))))
		return 0;

	memset (r, 0, sizeof (r));
	r->x = 1000 * (rand() / (double) RAND_MAX);
	r->y = 1000 * (rand() / (double) RAND_MAX);

	pollfd.fd = fd;

	fds[current_robots] = pollfd;
	all_robots[current_robots++] = r;
	return 1;
}

static volatile int timer;

void raise_timer (int sig)
{
	timer = 1;
}

void
process_robots ()
{
	int i, ret, rfd;
	struct pollfd *pfd;
	result_t result;
	char buf[STD_BUF];
	struct robot *robot;
	int to_talk;

	for (i = 0; i < max_robots; i++)
		fds[i].events = POLLIN | POLLOUT;

	do {
		to_talk = 0;
		for (i = 0; i < max_robots; i++) {
			if (fds[i].fd != -1) {
				to_talk++;
				rfd = fds[i].fd;
			}
		}

		if (to_talk <= 1) {
			if (to_talk == 0)
				ndprintf(stdout, "[GAME] Ended - No winner\n");
			else {
				ndprintf(stdout, "[GAME] Ended - Winner found\n");
				sockwrite(rfd, END, "Congratulations you are the winner!\n");
				while ((ret = read(rfd, buf, STD_BUF)) && ret != -1);
				close(rfd);
			}
			exit(EXIT_SUCCESS);
		}

		poll(fds, max_robots, 10);
		for (i = 0; i < max_robots; i++) {
			robot = all_robots[i];
			pfd = &fds[i];
			if (pfd->fd == -1) // Dead robot
				continue;

			if (pfd->revents & (POLLERR | POLLHUP)) { /* Error or disconnected robot -> kill */
				close(pfd->fd);
				pfd->fd = -1;
				kill_robot(robot);
				continue;
			}
			if (robot->damage >= 100) {
				sockwrite(pfd->fd, DEAD, "Sorry!");
				close(pfd->fd);
				pfd->fd = -1;
				continue;
			}

			if (!(pfd->revents & POLLIN))
				continue;

			if (!(pfd->revents & POLLOUT)) {
				close(pfd->fd);
				pfd->fd = -1;
				kill_robot(robot);
				continue;
			}

			ret = read(pfd->fd, buf, STD_BUF);
			switch (ret) {
				case -1:
					close(pfd->fd);
					pfd->fd = -1;
					kill_robot(robot);
					break;
				case 0:
					abort ();
				default:
					buf[ret] = '\0';
					result = execute_cmd(robot, buf);
					if (result.error) {
						sockwrite(pfd->fd, ERROR, "Violation of the protocol!\n");
						close(pfd->fd);
						pfd->fd = -1;
						kill_robot(robot);
					}
					else {
						if (result.cycle)
							pfd->events = 0;
						sockwrite(pfd->fd, OK, "%d", result.result);
					}
					break;
			}
		}
	} while (!timer);
}

void
server_start (char *hostname, char *port)
{
	int sockd, ret, fd, i, opt = 1;
	struct addrinfo *ai, *runp, hints;
	struct sockaddr *addr;
	socklen_t addrlen = sizeof(addr);
	double start, end;
    
	memset (&hints, 0x0, sizeof (hints));
	hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;
	hints.ai_socktype = SOCK_STREAM;

	ndprintf(stdout, "[SERVER] Starting Server at %s:%s\n[INFO] Number of players: %d\n", hostname, port, max_robots);

	if ((ret = getaddrinfo(hostname, port, &hints, &ai)))
		ndprintf_die(stderr, "[ERROR] getaddrinfo('%s', '%s'): %s\n", hostname, port, gai_strerror(ret));
	if (!ai)
		ndprintf_die(stderr, "[ERROR] getaddrinf(): couldn't fill the struct!\n");

	runp = ai;

	while (runp) {
		sockd = socket(runp->ai_family, runp->ai_socktype, runp->ai_protocol);
		if (sockd != -1)
			break;
		runp = runp->ai_next;
	}
	if (sockd == -1)
		ndprintf_die(stderr, "[ERROR] socket(): Couldn't create socket!\n");

	/* To close the port after closing the socket */
	if (setsockopt(sockd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof (opt)) == -1)
		ndprintf_die(stderr, "[ERROR] setsockopt(): %s\n", strerror(errno));
	if (bind(sockd, runp->ai_addr, runp->ai_addrlen))
		ndprintf_die(stderr, "[ERROR] bind(): %s\n", strerror(errno));
	if (listen(sockd, max_robots))
		ndprintf_die(stderr, "[ERROR] listen(): %s\n", strerror(errno));
	if (!(fds = (struct pollfd *) malloc (max_robots * sizeof(struct pollfd))))
		ndprintf_die(stderr, "[ERROR] Coulnd't malloc space for fds!\n");
	
	while (1) { /* Wait for all the clients to connect */
		fd = accept(sockd, (struct sockaddr *) &addr, &addrlen);
		if (!create_client(fd))
			sockwrite(fd, ERROR, "Couldn't duplicate the FD\n");
		if (current_robots >= max_robots)
			break;
	}
	ndprintf(stdout, "[GAME] Starting. All clients connected!\n");
	for (i = 0; i < max_robots; i++)
		sockwrite(fds[i].fd, START, "Let's play!");

	signal (SIGALRM, raise_timer);
}

void server_cycle (SDL_Event *event)
{
	int i;
	if (current_cycles >= max_cycles) {
		for (i = 0; i < max_robots; i++) {
			if (fds[i].fd != -1) {
				sockwrite(fds[i].fd, DRAW, "Max cycles reached!\n");
				close(fds[i].fd);
			}
		}
		ndprintf(stdout, "[GAME] Ended - Draw!\n");
		exit(EXIT_SUCCESS);
	}
	current_cycles++;
	struct itimerval itv;
	itv.it_interval.tv_sec = 0;
	itv.it_interval.tv_usec = 0;
	itv.it_value.tv_sec = 0;
	itv.it_value.tv_usec = 10000;
	setitimer (ITIMER_REAL, &itv, NULL);
	timer = 0;
	cycle();
	update_display(event);
	process_robots();
}

void
usage (char *prog, int retval)
{
	printf("Usage %s [-n <clients> -H <hostname> -P <port> -d]\n"
		"\t-n <clients>\tNumber of clients to start the game (has to be bigger than 1) (Default: 5)\n"
		"\t-H <hostname>\tSpecifies hostname (Default: 127.0.0.1)\n"
		"\t-P <port>\tSpecifies port (Default: 4300)\n"
		"\t-d\tEnables debug mode\n", prog);
	exit(retval);
}

int 
server_init (int argc, char *argv[])
{
	int retval;

	char *port = STD_PORT, *hostname = STD_HOSTNAME;

	while ((retval = getopt(argc, argv, "dn:hH:P:c:")) != -1) {
		switch (retval) {
			case 'c':
				max_cycles = atoi(optarg);
				break;	
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

	if (argc > optind) /* || !hostname || !port)*/
		usage(argv[0], EXIT_FAILURE);

	if (max_robots <= 1)
		max_robots = STD_CLIENTS;

	if (max_cycles <= 1)
		max_cycles = STD_CYCLES;

	all_robots = (struct robot **) malloc(max_robots * sizeof(struct robot *));

	server_start(hostname, port);
	return 0;
}
