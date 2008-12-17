#include "robotserver.h"
#import <stdlib.h>
#import <stdio.h>
#include <math.h>

int 
kill_robot(struct robot * r) {
 /* TODO  damage = 100. and bla bla */
	r->damage = 100;
}

int 
loc_x (struct robot * r) 
{
	return r->x;
}

int 
loc_y (struct robot * r) 
{
	return r->y;
}

int 
speed (struct robot *r)
{
	return r->speed;
}

int
damage (struct robot *r)
{
	return r->damage;
}

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
	int x, y;
	x = x2 - x1;
	y = y2 - y1;
	return sqrt(x*x + y*y);
}

int
compute_angle(int x1, int y1, int x2, int y2)
{
	return standardizeDegree(-atan2(y2 - y1,x2 - x1) * 180/M_PI);
}

int
scan (struct robot *r, int degree, int resolution)
{
	int min_distance = 1500;
	int posx = r->x;
	int posy = r->y;
	int n_robots = 2; //sizeof(**all_robots) / sizeof(struct robot);
	//printf("n_robots %d\n", n_robots);
	//printf("x %d\n", posx);
	//printf("y %d\n", posy);
	
	int i,distance,angle_between_robots,upper_angle,bottom_angle;

	if(resolution > 10 && resolution < 0){
		return -1;
	}
	
	degree = standardizeDegree(degree);
	//printf("degree %d\n", degree);
	upper_angle = degree + resolution;
	bottom_angle = degree - resolution;

	for(i = 0; i < n_robots; i++){
		angle_between_robots = compute_angle(posx, posy, all_robots[i]->x, all_robots[i]->y);
		//printf("name: %s angle_b2en %d\n",all_robots[i]->name, angle_between_robots);
		if(angle_between_robots <= upper_angle && angle_between_robots >= bottom_angle){
			distance =  getDistance(posx, posy, all_robots[i]->x, all_robots[i]->y);
			if(distance < min_distance && distance != 0){
				min_distance = distance;
			}
		}
	}
	if(1500 == min_distance) return 0;
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
	x = cos(degree * 180/M_PI) * range + r->x;
	y = sin(degree * 180/M_PI) * range + r->y;
	
	for(i = 0; i < n_robots; i++){
		distance_from_center = getDistance(all_robots[i]->x, all_robots[y]->y, x, y);
		if(distance_from_center <= 5)
			all_robots[i]->damage += 10;
		else if(distance_from_center <= 20)
			all_robots[i]->damage += 5;
		else if(distance_from_center <= 40)
			all_robots[i]->damage += 3;
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
	if(r->speed >= 50)
		degree = r->degree;
	r->target_speed = speed;
	r->degree = degree;
}

/*
void
cycle(){
	int i;
	for(i = 0; i < sizeof(*all_robots) / sizeof(struct robot); i++){
		cycle_robot (&all_robots[i]);
	}
}

static void
cycle_robot(struct robot *r)
{
	r->x += cos(degree * 180/M_PI) + masiar;
	r->y += sin(degree * 180/M_PI) + masiar;
	r->speed = masiar;
}
*/
/*
int
main ()
{
	struct robot walle = {
		"Wall-E",
		100, 100,
		0, 0, 0, 0, 45, 45,
		{
			{ 0, 0, 0 },
			{ 0, 0, 0 }			
		}
	};
	
	struct robot eve = {
		"Eve",
		500, 500,
		0, 0, 0, 0, 0, 0,
		{
			{ 0, 0, 0 },
			{ 0, 0, 0 }			
		}
	};
	
	struct robot *robogang[2];
	robogang[0] = &walle;
	robogang[1] = &eve;
	
	all_robots = robogang;
	
	int distance = scan(&walle, 315, 10);
	printf("Eve is at %dm from Walle\n", distance);
	
	distance = scan(&walle, 305, 9);
	printf("Walle does not know where Eve is. The scan returns %dm\n", distance);
	
	
}*/
