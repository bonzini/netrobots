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

int debugc = 1;


int serverfd;

int
client_init(char * remotehost, char * port) 
{
	int result = 1;
	int ret, sock;
	struct addrinfo * ai;
	struct addrinfo hints;
	
	memset (&hints, '\0', sizeof(hints));
	hints.ai_flags = AI_ADDRCONFIG;
	hints.ai_socktype = SOCK_STREAM;
	
	if(ret = getaddrinfo (remotehost, port, &hints, &ai)) {
		printf("[ERROR] getaddrinfo\n");
		//ndprintf_die(stderr, "[ERROR] getaddrinfo('%s', '%s'): %s\n",remotehost, port, gai_strerror(ret));
	}
	if(!ai) {
		printf("[ERROR] couldn't fill struct\n");
		//ndprintf_die(stderr, "[ERROR] getaddrinf(): couldn't fill the struct!\n");
	}
	if(debugc)printf("addrinfo OK\n");
	struct addrinfo *runp = ai;
	runp = ai;
	while (runp) {
		sock = socket(runp->ai_family, runp->ai_socktype, runp->ai_protocol);
		printf("opened socket");
		if (sock != -1) {
			if(debugc) printf("[NETWORK] connecting to server...\n");
			if(connect (sock, runp->ai_addr, runp->ai_addrlen) == 0) {
				if(debugc) printf("[NETWORK] connected to server\n");
				result = 0;
				break;
			}
			close(sock);	
		}
		runp = runp->ai_next;
	}

	freeaddrinfo(ai);
	serverfd = sock;
	return result;
}

int 
get_resp_value() 
{
	/*
	char resp[STD_RESP_LEN];
	int count;
	count = read(serverfd, resp, STD_RESP_LEN);
	if(count > 0 && resp[0]) {
		char ** argv; 
		int result;
		str_to_argv(resp, &argv);
		result = atoi(argv[1]);
		free_argv(&argv);
		return result;
	}
	else return -1;
*/
}

int
scan (int degree,int resolution)
{
	/* Send the command */
	sockwrite(serverfd, SCAN, "%d %d", degree, resolution);
	/* Retrieve response : TODO ... */
	return get_resp_value();	
}

int
cannon (int degree,int range)
{
	char argv[4] = { degree, ' ', range, '\0'};
	sockwrite(serverfd, CANNON, argv);
	return get_resp_value();
}

void
drive (int degree,int speed)
{
	char argv[4] = { degree, ' ', speed, '\0'};
	sockwrite(serverfd, DRIVE, argv);
	get_resp_value();
}

int
damage()
{
	sockwrite(serverfd, DAMAGE, NULL);
	return get_resp_value();
}

void
cycle ()
{
	sockwrite(serverfd, CYCLE, NULL);
	get_resp_value();

}

int
speed()
{
	sockwrite(serverfd, SPEED, "");
	return get_resp_value();
}

int
loc_x()
{
	sockwrite(serverfd, LOC_X,"");
	return get_resp_value();
}

int
loc_y()
{
	sockwrite(serverfd, LOC_Y, "");
	return get_resp_value();
}
