#include "robots.h"
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include "../server/net_utils.h"

int
client_init(char * remotehost, char * port) {
	int result = 0;
	int ret;
	struct addrinfo * ai;
	struct addrinfo hints;
	
	memset (&hints, '\0', sizeof(hints));
	hints.ai_flags = AI_ADDRCONFIG;
	hints.ai_socktype = SOCK_STREAM;
	
	if(ret = getaddrinfo (remotehost, port, &hints, &ai)) {
		//ndprintf_die(stderr, "[ERROR] getaddrinfo('%s', '%s'): %s\n",remotehost, port, gai_strerror(ret));
	}
	if(!ai) {
		//ndprintf_die(stderr, "[ERROR] getaddrinf(): couldn't fill the struct!\n");
	}
	struct addrinfo *runp = ai;
	int sock = socket (runp->ai_family, runp->ai_socktype,
					   runp->ai_protocol);
	runp = ai;
	while (runp) {
		sock = socket(runp->ai_family, runp->ai_socktype, runp->ai_protocol);
		if (sock != -1)
			break;
		runp = runp->ai_next;
	}
}

int send_cmd(/* ... */) {
	/*
	if (sock == -1)
		ndprintf_die(stderr, "[ERROR] socket(): Couldn't create socket!\n");
	
	if(connect(sock
	*/
}


int
scan (int degree,int resolution)
{
	
}

int
cannon (int degree,int range)
{
}

void
drive (int degree,int speed)
{
}

int
damage()
{
}

void
cycle ()
{
}

int
speed()
{
}

int
loc_x()
{
}

int
loc_y()
{
}
