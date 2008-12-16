#ifndef ROBOTSERVER_H
#define ROBOTSERVER_H 1

struct missile {
  int x, y;
}

struct robot {
  int fd;

  char *name;

  int x, y;
  int damage;
  int speed;
  struct missile cannon[2];

  /* info on what was done... */
};

struct robot *all_robots;
int max_robots;

/* Interface from networking code to game logic.  */

int scan (struct robot *r, int degree, int resolution);
int cannon (struct robot *r, int degree, int range);
void drive (struct robot *r, int degree, int speed);

#endif
