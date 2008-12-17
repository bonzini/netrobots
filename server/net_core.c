#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <stdio.h>

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
	r->x = r->y = r->speed = r->damage = 0;
	r->name = NULL;

	pollfd.fd = fd;
	pollfd.events = POLLIN | POLLOUT;

	fds[current_robots] = pollfd;
	all_robots[current_robots++] = r;
	return 1;
}

void
process_robots ()
{
	int i, ret, to_talk = max_robots;
	struct pollfd pfd;
	result_t result;
	char *buf;

	for (i = 0; i < max_robots; i++)
		all_robots[i]->take_cmd = true;

	while (poll(fds, max_robots, -1) > 0 && to_talk > 0) {
		to_talk = 0;
		for (i = 0; i < max_robots; i++) {
			pfd = fds[i];
			if (pfd.fd == -1) // Dead robot
				continue;
			if (pfd.revents & POLLERR || pfd.revents & POLLHUP) { /* Error or disconnected robot -> kill */
				pfd.fd = -1;
				// kill_robot(i);
				continue;
			}
			else if (pfd.revents & POLLOUT == 0 || !all_robots[i]->take_cmd)
				continue;
			if (pfd.revents & POLLIN) {
				buf = (char *) malloc(STD_BUF * sizeof(char));
				ret = read(pfd.fd, buf, STD_BUF);
				switch (ret) {
					case -1:
					case 0:
						break;
					default:
						result = execute_cmd(buf);
						if (result.error) {
							pfd.fd = -1;
							// kill_robot(i);
						}
						else {
							if (!result.cycle)
								to_talk++;
							//answer
						}
						break;
				}
			}
		}
	}
}

void
server_init (char *hostname, char *port)
{
	int sockd, ret, fd, i;
	struct addrinfo *ai, *runp, hints;
	struct sockaddr *addr;
	socklen_t addrlen = sizeof(addr);
    
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
	for (i = 0; i < max_robots; i++)
		sockwrite(fds[i].fd, START, NULL);
	while (1) {
		// move_robots();
		// fire_missiles();
		process_robots();
		// update_display();
		// sleep();
	}
}
