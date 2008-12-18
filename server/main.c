/* Main program.
   Paolo Bonzini, August 2008.

   This source code is released for free distribution under the terms
   of the GNU General Public License.  */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "drawing.h"
#include "anim.h"
#include "field.h"
#include "robotserver.h"

struct robot r;


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
  x = xx - loc_x (&r);
  y = yy - loc_y (&r);
  return atan2 (y, x) * 180 / 3.14159;
}


/* go - go to the point specified */
void go (int dest_x, int dest_y, cairo_t *cr)
{
  int course;
  course = plot_course (dest_x, dest_y);
  drive (&r, course, 25);
  while (distance (r.x, r.y, dest_x, dest_y) > 50)
{    cycle ();
  do_map (cr, NULL); 
      draw_gl ();}
  drive (&r, course, 0);
  while (r.speed > 0)
    cycle ();
}


// fine esempio

int
main (int argc, char **argv)
{
	unsigned int i = 0, start_ticks;
	max_robots = 1;

	struct robot *robogang[2];
	robogang[0] = &r;
	
	all_robots = robogang;
	


  /* initialize SDL and create as OpenGL-texture source */
  cairo_t *cairo_context = init_gl ();

  init_anim ();
  init_map ();

  start_ticks = SDL_GetTicks ();

    
	
  /* enter event-loop */
  for (;;)
		{
		int x = rand () / (double) RAND_MAX * 1000;
		int y = rand () / (double) RAND_MAX * 1000;
		go (x, y, cairo_context);	/* go somewhere in the field */
      
		SDL_Event event;
		i++;
		draw_gl ();
		event.type = -1;
		SDL_PollEvent (&event);

		/* check for user hitting close-window widget */
		if (event.type == SDL_QUIT)
			break;

		/* Call functions here to parse event and render on cairo_context...  */
		do_anim (cairo_context, &event);
		do_map (cairo_context, &event);
    }
	

  printf ("%.2f fps\n", (i * 1000.0) / (SDL_GetTicks () - start_ticks));

  /* clear resources before exit */
  destroy_cairo_context (cairo_context);
  free_gl ();

  return 0;
}
