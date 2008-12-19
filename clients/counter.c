/* counter */
/* scan in a counter-clockwise direction (increasing degrees) */
/* moves when hit */

#include "robots.h"
#include <stdlib.h>

/* run moves around the center of the field */
void
run ()
{
  enum { EAST, NORTH, WEST, SOUTH } last_dir;
  int x, y;
  int i;
  x = loc_x ();
  y = loc_y ();
  if (last_dir == EAST || last_dir == WEST)
    last_dir = (y > 500 ? SOUTH : NORTH);
  else
    last_dir = (x > 500 ? WEST : EAST);

  switch (last_dir)
    {
    case SOUTH:
      drive (90, 100);
      while (y - 100 < loc_y () && i++ < 100)
	cycle ();
      break;
    case NORTH:
      drive (270, 100);
      while (y + 100 > loc_y () && i++ < 100)
	cycle ();
      break;
    case WEST:
      drive (180, 100);
      while (x - 100 < loc_x () && i++ < 100)
	cycle ();
      break;
    case EAST:
      drive (0, 100);
      while (x + 100 > loc_x () && i++ < 100)
	cycle ();
      break;
    }

  drive (0, 0);
}

#define RES			3
#define DAMAGE_THRESHOLD	1

int
main ()
{
  int angle, range;
  int d;
  int i;
  while (1)
    {
      run ();
      d = damage ();
      angle = rand () / (double) RAND_MAX * 360;

      while (damage () < d + DAMAGE_THRESHOLD)
	{
	  range = scan (angle, RES);
	  if (range == 0)
	    angle = (angle + RES * 2) % 360;

	  else if (range > 700)
	    {			/* out of range, head toward it */
	      drive (angle, 50);
	      i = 1;
	      while (i++ < 50)	/* use a counter to limit move time */
		cycle ();
	      drive (angle, 0);
	      angle -= 3;
	    }
	  else
	    {
printf ("found at %d dist %d\n", angle, range);
	      while (cannon (angle, range) == 0)	/* fire first shot */
		cycle ();
	      while (cannon (angle, range) == 0)	/* fire second shot */
		cycle ();
	      angle -= 15;
	    }
	}
    }
}
