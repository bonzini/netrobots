#include "robots.h"
main ()
{
  int direction;
  direction = 90;
  while (1)
    {
      if (speed () == 0)
	drive (direction, 100);
      if ((direction == 90 && loc_y () > 575)
	  || (direction == 270 && loc_y () < 425)
	  || (direction == 180 && loc_x () < 150)
	  || (direction == 0 && loc_x () > 850))
	{
	  drive (direction, 0);
	  direction = (direction + 90) % 360;
	}
      else
	{
	  int range;
	  if (range = scan (0, 10)) cannon (0, range);
	  else if (range = scan (30, 10)) cannon (30, range);
	  else if (range = scan (60, 10)) cannon (60, range);
	  else if (range = scan (90, 10)) cannon (90, range);
	  else if (range = scan (120, 10)) cannon (120, range);
	  else if (range = scan (150, 10)) cannon (150, range);
	  else if (range = scan (180, 10)) cannon (180, range);
	  else if (range = scan (210, 10)) cannon (210, range);
	  else if (range = scan (240, 10)) cannon (240, range);
	  else if (range = scan (270, 10)) cannon (270, range);
	  else if (range = scan (300, 10)) cannon (300, range);
	  else if (range = scan (330, 10)) cannon (330, range);
	}
    }
}
