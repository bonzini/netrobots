#ifndef ROBOTSERVER_H
#define ROBOTSERVER_H 1

#define RELOAD_RATIO 300
#define SPEED_RATIO 0.04
#define BREAK_DISTANCE (0.7 / SPEED_RATIO)

struct cannon {
	int timeToReload;
	int x, y;
};

struct robot {
  // int fd; Should not be needed as it is synchronized with the array of fds
  char *name;
  double x, y;
  int damage;
  int speed;
  int break_distance;
  int target_speed;
  int degree;
  int cannon_degree;
  int radar_degree;
  struct cannon cannon[2];
  float * color; 	

  /* info on what was done... */
};

struct robot **all_robots;
int max_robots;

/* Interface from networking code to game logic.  */

int scan (struct robot *r, int degree, int resolution);
int cannon (struct robot *r, int degree, int range);
void drive (struct robot *r, int degree, int speed);
void cycle (void);
int loc_x (struct robot *r);
int loc_y (struct robot *r);
int speed (struct robot *r);
int damage (struct robot *r);
void kill_robot (struct robot *r);

#endif
