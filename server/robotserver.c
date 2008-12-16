#include <robotserver.h>
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
distance(int x1, int y1, int x2, int y2)
{
	int x,y;
	x = x2 - x1;
	y = y2 - y1;
	return sqrt(x*x + y*y);
}

int
compute_angle(int x1, int y1, int x2, int y2)
{
	return pi/180  * atan2(y2 - y1,x2 - x1);
}

int
scan (struct robot *r, int degree, int resolution)
{
	int min_distance = 1500;
	int position-x = r->x;
	int position-y = r->y;
	int n_robots = sizeof(*all_robots)/sizeof(struct robot);
	int i,distance,angle_between_robots,upper_angle,bottom_angle;

	if(resolution > 10 && resolution < 0)
		return -1;
	
	degree = standardizeDegree(degree);
	upper_angle = degree+resolution;
	bottom_angle = degree-resolution;

	for(i=0;i<n_robots;i++){
		angle_between_robots = distance(all_robots[i]->x, position-x,all_robots[i]->y, position-y);
		if(angle_between_robots <= upper_angle && angle_between_robots >= bottom_angle){
			dist = distance(all_robots[i]->x, position-x,all_robots[i]->y, position-y);
			if(distance < min_distance){
				min_distance = distance;
			}
		}
	}

	if(nearer == NULL){
		return 0;
	} else {
		return dinstance;
	}
}

int
cannon (struct robot *r, int degree, int range)
{
	int n_robots = sizeof(*all_robots)/sizeof(struct robot);
	int x,y,i;

	if(degree > 700)
		degree = 700;
	
	degree = standardizeDegree(degree);
	
      	x = sin(degree*pi/180) + r->x;
	y = cos(degree*pi/180) + r->y;
	
	for(i=0;i<n_robots;i++){
		distance_from_center = distance(all_robots[i]->x,all_robots[y]->y,x,y);
		if(distance <= 5){
			all_robots[i]->damage -= 10;
		} else if(distance <= 20){
			all_robots[i]->damage -= 5;
		} else if(distance <= 40){
			all_robots[i]->damage -= 3;
		}
	}
}

int
drive (struct robot *r, int degree, int speed)
{
}

int
main ()
{
}
