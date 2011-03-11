/* Interface between Cairo and SDL.
   Paolo Bonzini, August 2008.

   This source code is released for free distribution under the terms
   of the GNU General Public License.  */

#include <stdio.h>
#include <stdlib.h>

#include "drawing.h"

#include "SDL.h"

static cairo_t *
create_cairo_context_1 (unsigned char *buffer)
{
  cairo_t *cairo_context;
  cairo_surface_t *surface;

  /* create cairo-surface/context to act as SDL source */
  surface = cairo_image_surface_create_for_data (buffer,
						 CAIRO_FORMAT_ARGB32,
						 WIN_WIDTH, WIN_HEIGHT,
						 4 * WIN_WIDTH);

  if (cairo_surface_status (surface) != CAIRO_STATUS_SUCCESS)
    {
      printf ("couldn't create cairo surface\n");
      exit (1);
    }

  cairo_context = cairo_create (surface);
  if (cairo_status (cairo_context) != CAIRO_STATUS_SUCCESS)
    {
      printf ("couldn't create cairo context\n");
      exit (1);
    }

  return cairo_context;
}

cairo_t *
create_cairo_context (void)
{
  unsigned char *buffer;
  buffer = calloc (4 * WIN_WIDTH * WIN_HEIGHT, sizeof (char));
  return create_cairo_context_1 (buffer);
}

void
destroy_cairo_context (cairo_t *cairo_context)
{
  cairo_surface_t *surface = cairo_get_target (cairo_context);

#if CAIRO_VERSION_MAJOR > 1 || (CAIRO_VERSION_MAJOR == 1 && CAIRO_VERSION_MINOR >= 6)
  /* Memory leak for old cairo versions, but you should not create
     and destroy surfaces on every frame anyway.  */
  free (cairo_image_surface_get_data (surface));
#endif

  cairo_surface_destroy (surface);
  cairo_destroy (cairo_context);
}


static SDL_Surface *sdl_surface, *window_surface;

cairo_t *
init_sdl (void)
{
  /* init cairo.  */
  unsigned char *buffer = calloc (4 * WIN_WIDTH * WIN_HEIGHT, sizeof (char));
  cairo_t *cairo_context = create_cairo_context_1 (buffer);

  /* init SDL */
  if ((SDL_Init (SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1))
    {
      printf ("Could not initialize SDL: %s.\n", SDL_GetError ());
      exit (-1);
    }

  /* set window title */
  SDL_WM_SetCaption (WIN_TITLE, NULL);

  window_surface = SDL_SetVideoMode (WIN_WIDTH, WIN_HEIGHT, 0, SDL_DOUBLEBUF);

  /* did we get what we want? */
  if (!window_surface)
    {
      printf ("Couldn't open SDL window: %s\n", SDL_GetError ());
      exit (-2);
    }

  sdl_surface = SDL_CreateRGBSurfaceFrom (buffer, WIN_WIDTH, WIN_HEIGHT, 32,
					  WIN_WIDTH * 4,
					  0xFF0000, 0xFF00, 0xFF, 0);
  if (!sdl_surface)
    {
      printf ("Couldn't create SDL surface: %s\n", SDL_GetError ());
      exit (-2);
    }

  return cairo_context;
}

void draw_sdl (void)
{
  SDL_BlitSurface (sdl_surface, NULL, window_surface, NULL);
  SDL_Flip (window_surface);
}

void
free_sdl (void)
{
  SDL_Quit ();
}
