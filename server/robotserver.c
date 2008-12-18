#include "robotserver.h"
#import <stdlib.h>
#import <stdio.h>
#include <math.h>

void
kill_robot(struct robot *r) {
	r->x = -1000;
	r->y = -1000;
	r->damage = 100;
}

int 
loc_x (struct robot *r) 
{
	return r->x;
}

int 
loc_y (struct robot *r) 
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
	int i,angle_between_robots,upper_angle,bottom_angle;
	int distance;
	//printf("x %d\n", posx);
	//printf("y %d\n", posy);

	if(resolution > 10 && resolution < 0){
		return -1;
	}
	
	degree = standardizeDegree(degree);
	
	r->radar_degree = degree;
	
	//printf("degree %d\n", degree);
	upper_angle = degree + resolution;
	bottom_angle = degree - resolution;

	for(i = 0; i < max_robots; i++){
		if(all_robots[i]->damage < 100){
			angle_between_robots = compute_angle(posx, posy, all_robots[i]->x, all_robots[i]->y);
			//printf("name: %s angle_b2en %d\n",all_robots[i]->name, angle_between_robots);
			if(angle_between_robots <= upper_angle && angle_between_robots >= bottom_angle){
				distance =  getDistance(posx, posy, all_robots[i]->x, all_robots[i]->y);
				if(distance < min_distance && distance != 0){
					min_distance = distance;
				}
			}
		}
	}
	if(1500.0 == min_distance) return 0;
	return min_distance;
}

int
cannon (struct robot *r, int degree, int range)
{
	int i,freeSlot;
	int distance_from_center, x, y;
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
	
	r->cannon_degree = degree;
	
	printf("Degree %d, Cos %g, Sin %g\n", degree, cos(degree * M_PI/180), sin(degree * M_PI/180));
	
	x = fabs(cos(degree * M_PI/180)) * range + r->x;
	y = fabs(sin(degree * M_PI/180)) * range + r->y;
	
	printf("x%d, y%d\n", x,y);
	
	for(i = 0; i < max_robots; i++){
		if(all_robots[i]->damage < 100){
			distance_from_center = getDistance(all_robots[i]->x, all_robots[i]->y, x, y);
			if(distance_from_center <= 5)
				all_robots[i]->damage += 10;
			else if(distance_from_center <= 20)
				all_robots[i]->damage += 5;
			else if(distance_from_center <= 40)
				all_robots[i]->damage += 3;
		}
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
	r->x += cos(degree * 180/M_PI) + (r->speed / 10) * 4;
	r->y += sin(degree * 180/M_PI) + (r->speed / 10) * 4;
	r->speed = (r->speed / 10) * 4;
}
*/

int
main ()
{
	struct robot walle = {
		"Wall-E",
		true,
		100, 100,
		0, 0, 0, 0, 0, 0,
		{
			{ 0, 0, 0 },
			{ 0, 0, 0 }			
		}
	};
	
	struct robot eve = {
		"Eve",
		true,
		500, 500,
		0, 0, 0, 0, 0, 0,
		{
			{ 0, 0, 0 },
			{ 0, 0, 0 }			
		}
	};
	
	struct robot masiar = {
		"Bubi",
		true,
		300, 300,
		0, 0, 0, 0, 0, 0,
		{
			{ 0, 0, 0 },
			{ 0, 0, 0 }			
		}
	};
	
	struct robot *robogang[3];
	robogang[0] = &walle;
	robogang[1] = &eve;
	robogang[2] = &masiar;
	
	all_robots = robogang;
	max_robots = 3;
	
	int distance = scan(&walle, 315, 10);
	printf("Closest robot is at %dm from Walle\n", distance);
		
	int result = cannon(&walle, 315, distance);
	printf("Walle shots the closest robot! Its damage is now %d\n", masiar.damage);	
	
	distance = scan(&walle, 305, 9);
	printf("Walle seems to be alone. The scan returns %dm\n", distance);
}
