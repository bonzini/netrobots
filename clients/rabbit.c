/* rabbit */
/* rabbit runs around the field, randomly */
/* and never fires;  use as a target */

#include "robots.h"
#include <stdlib.h>
#include <math.h>

/* distance forumula */
int
distance (int x1, int y1, int x2, int y2)
{
  int x, y;
  x = x1 - x2;
  y = y1 - y2;
  return sqrt ((x * x) + (y * y));
}


/* plot_course - figure out which heading to go */
int
plot_course (int xx, int yy)
{
  int x, y;
  x = loc_x () - xx;
  y = loc_y () - yy;
  return atan2 (y, x) * 180 / M_PI;
}


/* go - go to the point specified */
void go (int dest_x, int dest_y)
{
  int course;
  course = plot_course (dest_x, dest_y);
  drive (course, 25);
  while (distance (loc_x (), loc_y (), dest_x, dest_y) > 50)
    cycle ();
  drive (course, 0);
  while (speed () > 0)
    cycle ();
}



int
main ()
{
  while (1)
    {
      int x = rand () / (double) RAND_MAX * 1000;
      int y = rand () / (double) RAND_MAX * 1000;
      go (x, y);	/* go somewhere in the field */
    }
}
