/* Main program.
   Paolo Bonzini, August 2008.

   This source code is released for free distribution under the terms
   of the GNU General Public License.  */

#include <stdio.h>
#include <stdlib.h>

#include "drawing.h"
#include "anim.h"
#include "field.h"

int
main (int argc, char **argv)
{
  unsigned int i = 0, start_ticks;

  /* initialize SDL and create as OpenGL-texture source */
  cairo_t *cairo_context = init_gl ();

  init_anim ();
  init_map ();

  start_ticks = SDL_GetTicks ();

  /* enter event-loop */
  for (;;)
    {
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
