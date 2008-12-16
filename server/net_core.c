#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/poll.h>

#include "net_utils.h"
#include "net_defines.h"
#include "robotserver.h"

int current_robots = 0;
struct pollfd **fds;

extern int max_robots;
extern struct robot **all_robots;

struct pollfd *
create_pollfd (int fd)
{
	struct pollfd *pollfd;
	if (!(pollfd = (struct pollfd *) malloc (sizeof(struct pollfd))))
		return NULL;
	pollfd->events = POLLIN;
	pollfd->fd = fd;
	return pollfd;
}

int
create_client (int fd)
{
	if (fd == -1)
		return 0;
	struct robot *r;
	struct pollfd *pollfd;
	if (!(r = (struct robot *) malloc (sizeof(struct robot))))
		return 0;
	if (!(pollfd = create_pollfd(fd)))
		return 0;
	r->x = r->y = r->speed = r->damage = 0;
	r->name = NULL;
	fds[current_robots] = pollfd;
	all_robots[current_robots++] = r;
	return 1;
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

	if (!(fds = (struct pollfd **) malloc (max_robots * sizeof(struct pollfd *))))
		ndprintf_die(stderr, "[ERROR] Coulnd't malloc space for fds!\n");

	while (1) { /* Wait for all the clients to connect */
		fd = accept(sockd, (struct sockaddr *) &addr, &addrlen);
		if (!create_client(fd))
			sockwrite(fd, ERROR, "Couldn't duplicate the FD\n");
		if (current_robots >= max_robots)
			break;
	}
	for (i = 0; i < max_robots; i++)
		sockwrite(fds[i]->fd, START, NULL);
	while (1) {
		// Proces clients requests and answer
	}
}
