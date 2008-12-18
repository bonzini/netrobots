#ifndef ROBOTS_H
#define ROBOTS_H 1

#define STD_RESP_LEN 100
#define DEFAULT_REMOTEHOST "127.0.0.1"
#define DEFAULT_PORT "4300"

int scan (int degree,int resolution);
int client_init(char * remotehost, char * port);
int cannon (int degree,int range);
void drive (int degree,int speed);
int damage();
void cycle ();
int speed();
int loc_x();
int loc_y();

#endif
