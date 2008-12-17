#include "robots.h"
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include "../server/net_utils.h"
#include "../server/net_utils.c"
#include "../server/net_command_list.h"

#define STD_RESP_LEN 100
int serverfd;

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
		if (sock != -1) {
			if(connect (sock, runp->ai_addr, runp->ai_addrlen) == 0) {
				goto end;
			}
			close(sock);	
		}
		runp = runp->ai_next;
	}

	result = 1;
	freeaddrinfo(ai);
	//ndprintf_die(stderr, "[ERROR] socket(): Couldn't create socket!\n");
	
	end:
	serverfd = sock;
	return result;
}

int
scan (int degree,int resolution)
{
	/* Send the command */
	char argv[4] = { degree, ' ', resolution, '\0'};
	sockwrite(serverfd, SCAN, argv);
	/* Retrieve response : TODO ... */
	char resp[STD_RESP_LEN];
	read(serverfd, resp, STD_RESP_LEN);
	return resp[0]; /* Returns the status */
	
}

int
cannon (int degree,int range)
{
	/*
	char argv[4] = { degree, ' ', range, '\0'};
	sockwrite(serverfd, CANNON, argv);
	 */
}

void
drive (int degree,int speed)
{/*
	char argv[4] = { degree, ' ', speed, '\0'};
	sockwrite(serverfd, DRIVE, argv);
 */
}

int
damage()
{
	/*
	sockwrite(serverfd, DAMAGE, "");
	*/
}

void
cycle ()
{
	/*
	sockwrite(serverfd, CYCLE, "");
	 */

}

int
speed()
{
	/*
	sockwrite(serverfd, SPEED, "");
	*/
}

int
loc_x()
{
	/*
	sockwrite(serverfd, LOC_X,"");
	*/
}

int
loc_y()
{
	/*
	sockwrite(serverfd, LOC_Y, "");
	 */
}
