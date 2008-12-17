/* sniper */
/* strategy: since a scan of the entire battlefield can be done in 90 */
/* degrees from a corner, sniper can scan the field quickly. */

#include "robots.h"
#include <math.h>

/* classical pythagorean distance formula */
int
distance (int x1, int y1, int x2, int y2)
{
  int x, y;
  x = x1 - x2;
  y = y1 - y2;
  return sqrt ((x * x) + (y * y));
}


/* plot course function, return degree heading to */
/* reach destination x, y; uses atan() trig function */
int
plot_course (int xx, int yy)
{
  int x, y;
  x = loc_x () - xx;
  y = loc_y () - yy;
  return atan2 (y, x) * 180 / M_PI;
}

void
go (int x, int y)
{
  /* find the heading we need to get to the desired corner */
  int angle = plot_course (x, y);

  /* start drive train, full speed */
  drive (angle, 100);

  /* keep traveling until we are within 100 meters */
  /* speed is checked in case we run into wall, other robot */
  /* not terribly great, since we are doing nothing while moving */
  while (distance (loc_x (), loc_y (), x, y) > 100 && speed () > 0)
    cycle ();

  /* cut speed, and creep the rest of the way */
  drive (angle, 20);
  while (distance (loc_x (), loc_y (), x, y) > 10 && speed () > 0)
    cycle ();

  /* stop drive, should coast in the rest of the way */
  drive (angle, 0);
}

/* new corner function to move to a different corner */
void
new_corner ()
{
  static int corner;
  corner = (corner + 1) % 4;	/* current corner */
  switch (corner)
    {
    case 0:			/* set new x,y and scan start */
      go (10, 10);
      break;
    case 1:
      go (10, 990);
      break;
    case 2:
      go (990, 990);
      break;
    case 3:
      go (990, 10);
      break;
    }
}


#define MAX(a, b)		((a) > (b) ? (a) : (b))

/* main */
int
main ()
{
  /* initialize the corner info */
  while (1)
    {				/* loop is executed forever */
      int range;		/* range to target */
      int hit;			/* check for targets in range */
      int dir, sc;		/* scan direction */
      int d;			/* last damage check */

      new_corner ();		/* start at a random corner */
      d = damage ();		/* get current damage */

      /* scan around sc.  */
      sc = plot_course (500, 500);
      dir = sc - 46;
      hit = 0;
      do
	{
	  dir += 2;
	  if (dir >= sc + 45)	/* at end of 90 degrees range...  */
	    {
	      if (!hit)		/* move if we hit */
		break;
	      dir = sc - 45;	/* ... or restart */
	    }

	  range = scan (dir, 1);	/* look at a direction */

	  /* keep firing while in range, tolerating several hits */
	  while (range <= 700 && range > 0 && damage () <= d + 15)
	    {
	      hit = 1;
	      cannon (dir, range);	/* fire! */
	      range = scan (dir, 1);	/* check target again */
	    }

	  if (hit)
	    dir = MAX (dir - 10, sc - 45);	/* back up scan after attack */
	}
      while (d == damage ());
    }
}
