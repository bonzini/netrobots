/* rook */
/* scan the battlefield like a rook, i.e., only 0,90,180,270 */
/* move horizontally only, but looks horz and vertically */

#include "robots.h"
#include <stdio.h>
int course;
int boundary;

void
change ()
{
  if (course == 0)
    {
      boundary = 5;
      course = 180;
    }
  else
    {
      boundary = 995;
      course = 0;
    }
  drive (course, 30);
}

/* look somewhere, and fire cannon repeatedly at in-range target */
void
look (int deg)
{
  static int d;
  int range;
  while ((range = scan (deg, 2)) > 0 && range <= 700)
    {
      drive (course, 0);
      cannon (deg, range);
      if (damage () >= d + 20)
	{
	  d = damage ();
	  change ();
	}
    }
}

int
main ()
{
  int y;

  /* move to center of board */
  if (loc_y () < 500)
    {
      y = 90;
      drive (90, 70);		/* start moving */
      while (500 - loc_y () > 20 && speed () > 0)	/* stop near center */
	cycle ();
    }
  else
    {
      y = 270;
      drive (270, 70);		/* start moving */
      while (loc_y () - 500 > 20 && speed () > 0)	/* stop near center */
	cycle ();
    }
  drive (y, 0);

  /* initialize starting parameters */
  course = 0;
  boundary = 995;
  drive (course, 30);

  /* main loop */
  while (1)
    {

      /* look all directions */
      look (0);
      look (90);
      look (180);
      look (270);

      /* if near end of battlefield, change directions */
      if (course == 0)
	{
	  if (loc_x () > boundary || speed () == 0)
	    change ();
	}

      else
	{
	  if (loc_x () < boundary || speed () == 0)
	    change ();
	}
    }
}
