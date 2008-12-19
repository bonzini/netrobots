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
	/* Convert 0.5 ... 999.5 to 0 ... 1000 */
	int x = (r->x - 0.5) * 1000 / 999;
	if (x < 0)
		x = 0;
	if (x > 999)
		x = 999;
	return x;
}

int 
loc_y (struct robot *r) 
{
	/* Convert 0.5 ... 999.5 to 0 ... 1000 */
	int y = (r->y - 0.5) * 1000 / 999;
	if (y < 0)
		y = 0;
	if (y > 999)
		y = 999;
	return y;
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
	
	//printf("Degree %d, Cos %g, Sin %g\n", degree, cos(degree * M_PI/180), sin(degree * M_PI/180));
	
	x = cos(degree * M_PI/180) * range + r->x;
	y = sin(degree * M_PI/180) * range + r->y;
	
	//printf("x%d, y%d\n", x,y);
	
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
		if(all_robots[i]->damage >= 100)
			kill_robot(all_robots[i]);
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
	
	if(speed > r->target_speed)
		r->speed = speed;
	r->target_speed = speed;
	r->degree = degree;
	r->break_distance = BREAK_DISTANCE;
}

#define TOL   (sin(M_PI/360))

static void
cycle_robot(struct robot *r)
{
	if(r->x >= 1000 + TOL || r->x <= -TOL || r->y >= 1000 + TOL || r->y <= -TOL){
		r->damage += 2;
		r->speed = 0;
		r->break_distance = 0;
		r->target_speed = 0;
	}
	if(r->damage < 100){
		if (r->x > 1000)
			r->x = 1000;
		if (r->x < 0)
			r->x = 0;
		if (r->y > 1000)
			r->y = 1000;
		if (r->y < 0)
			r->y = 0;
	}
		
	int i;
	for(i = 0; i < max_robots; i++)
		if((int)r->x == (int)all_robots[i]->x && (int)r->y == (int)all_robots[i]->y){
			if(all_robots[i] != r){
				r->damage += 2;
				r->speed = 0;
				r->break_distance = 0;
				r->target_speed = 0;
				all_robots[i]->damage += 2;
				all_robots[i]->speed = 0;
				all_robots[i]->break_distance = 0;
				all_robots[i]->target_speed = 0;
				if(r->x > 0){
					r->x -= 1;
				} else {
					r->x += 1;
				}
				if(r->y > 0){
					r->y -= 1;
				} else {
					r->y += 1;
				}
			}
		}
	
	r->x += cos(r->degree * M_PI/180) * r->speed * SPEED_RATIO;
	r->y += sin(r->degree * M_PI/180) * r->speed * SPEED_RATIO;
	
	//printf("Degree %d, Cos %g, Sin %g, Speed %d\n", r->degree , cos(r->degree  * M_PI/180), sin(r->degree  * M_PI/180), r->speed);
	
	if(r->break_distance == 0)
		r->speed = r->target_speed;
	
	if(r->target_speed < r->speed){
		r->speed += (r->target_speed - r->speed) / r->break_distance;
		r->break_distance--;
	}
	
	/*Decreasing the time to reload the missiles*/
	for(i = 0; i < 2; i++){
		if(r->cannon[i].timeToReload != 0)
			r->cannon[i].timeToReload--;
	}
}

void
cycle()
{
	int i;
	for(i = 0; i < max_robots; i++)
		if(all_robots[i]->damage < 100)
			cycle_robot(all_robots[i]);
}
