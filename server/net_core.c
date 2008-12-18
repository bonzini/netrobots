#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <stdio.h>
#include <sys/time.h>

#include "net_utils.h"
#include "net_defines.h"
#include "robotserver.h"

int current_robots = 0;
struct pollfd *fds;

extern int max_robots;
extern struct robot **all_robots;

int
create_client (int fd)
{
	struct pollfd pollfd;

	if (fd == -1)
		return 0;
	struct robot *r;
	if (!(r = (struct robot *) malloc (sizeof(struct robot))))
		return 0;
	r->x = 1000 * (rand() / (double) RAND_MAX);
	r->y = 1000 * (rand() / (double) RAND_MAX);
	r->speed = r->damage = 0;
	r->name = NULL;

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
server_init (char *hostname, char *port)
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
	while (1) {
		struct itimerval itv;
		itv.it_interval.tv_sec = 0;
		itv.it_interval.tv_usec = 0;
		itv.it_value.tv_sec = 0;
		itv.it_value.tv_usec = 10000;

		setitimer (ITIMER_REAL, &itv, NULL);
		timer = 0;
		cycle();

		//update_display();
		process_robots();
	}
	freeaddrinfo(ai);
	exit(EXIT_FAILURE);
}
