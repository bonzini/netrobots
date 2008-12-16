#ifndef ROBOTS_H
#define ROBOTS_H 1

int scan (int degree,int resolution);
int cannon (int degree,int range);
void drive (int degree,int speed);
int damage();
void cycle ();
int speed();
int loc_x();
int loc_y();

#endif
