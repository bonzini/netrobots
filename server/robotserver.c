#include "robotserver.h"
#import <stdlib.h>
#include <math.h>

int
standardizeDegree(int degree)
{	
	int result = degree;
	if(degree < 0){
		result =  360 - (-degree % 360);
	}
	return result % 360;
}

int
getDistance(int x1, int y1, int x2, int y2)
{
	int x,y;
	x = x2 - x1;
	y = y2 - y1;
	return sqrt(x*x + y*y);
}

int
compute_angle(int x1, int y1, int x2, int y2)
{
	return atan2(y2 - y1,x2 - x1) * 180/3.1416;
}

int
scan (struct robot *r, int degree, int resolution)
{
	int min_distance = 1500;
	int posx = r->x;
	int posy = r->y;
	int n_robots = sizeof(*all_robots)/sizeof(struct robot);
	int i,distance,angle_between_robots,upper_angle,bottom_angle;

	if(resolution > 10 && resolution < 0){
		return -1;
	}
	
	degree = standardizeDegree(degree);
	upper_angle = degree+resolution;
	bottom_angle = degree-resolution;

	for(i=0;i<n_robots;i++){
		angle_between_robots = getDistance(all_robots[i].x, posx,all_robots[i].y, posy);
		if(angle_between_robots <= upper_angle && angle_between_robots >= bottom_angle){
			distance =  getDistance(all_robots[i].x, posx, all_robots[i].y, posy);
			if(distance < min_distance){
				min_distance = distance;
			}
		}
	}
	return min_distance;
}

int
cannon (struct robot *r, int degree, int range)
{
	int n_robots = sizeof(*all_robots) / sizeof(struct robot);
	int x,y,i,freeSlot,distance_from_center;
	/* If the cannon is not reloading, meaning it's ready the robottino shoots otherwise break */
	for(freeSlot = 0; freeSlot < 2; freeSlot++)
		if(r->cannon[freeSlot].timeToReload == 0) break;
	
	if(freeSlot == 2){
		return 0;
	}
	
	/* If we reach that point the missile could be shot */
	if(range > 700)
		range = 700;
	degree = standardizeDegree(degree);
	x = sin(degree * 180/3.1416) * range + r->x;
	y = cos(degree * 180/3.1416) * range + r->y;
	
	for(i = 0; i < n_robots; i++){
		distance_from_center = getDistance(all_robots[i].x, all_robots[y].y, x, y);
		if(distance_from_center <= 5)
			all_robots[i].damage += 10;
		else if(distance_from_center <= 20)
			all_robots[i].damage += 5;
		else if(distance_from_center <= 40)
			all_robots[i].damage += 3;
	}
	
	r->cannon[freeSlot].timeToReload = RELOAD_RATIO;
	r->cannon[freeSlot].x = x;
	r->cannon[freeSlot].y = y;
	return 1;
}

void
drive (struct robot *r, int degree, int speed)
{
	degree = standardizeDegree(degree);
	if(speed >= 50)
		degree = r->degree;
	r->speed = speed;
	r->degree = degree;
}

int
main ()
{
}
