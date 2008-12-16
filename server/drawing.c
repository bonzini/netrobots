/* Interface between Cairo and SDL+OpenGL.
   Paolo Bonzini, August 2008.

   This source code is released for free distribution under the terms
   of the GNU General Public License.  */

#include <stdio.h>
#include <stdlib.h>

#include "drawing.h"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#undef GL_GLEXT_VERSION
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#include "SDL.h"
#include "SDL_opengl.h"

static cairo_t *
create_cairo_context_1 (unsigned char *buffer)
{
  cairo_t *cairo_context;
  cairo_surface_t *surface;

  /* create cairo-surface/context to act as OpenGL-texture source */
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


static unsigned char *texture_data;
static GLuint texture_format;
static GLuint texture_id;
static GLuint list_id;

cairo_t *
init_gl (void)
{
  SDL_Surface *p_surface_window = NULL;

  /* init cairo.  */
  unsigned char *buffer = calloc (4 * WIN_WIDTH * WIN_HEIGHT, sizeof (char));
  cairo_t *cairo_context = create_cairo_context_1 (buffer);
  cairo_surface_t *surface = cairo_get_target (cairo_context);
  texture_data = buffer;

  /* init SDL */
  if ((SDL_Init (SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1))
    {
      printf ("Could not initialize SDL: %s.\n", SDL_GetError ());
      exit (-1);
    }

  /* set window title */
  SDL_WM_SetCaption (WIN_TITLE, NULL);

  /* create "GL-context" */
  SDL_GL_SetAttribute (SDL_GL_RED_SIZE, 5);
  SDL_GL_SetAttribute (SDL_GL_GREEN_SIZE, 6);
  SDL_GL_SetAttribute (SDL_GL_BLUE_SIZE, 5);
  SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
  p_surface_window = SDL_SetVideoMode (WIN_WIDTH, WIN_HEIGHT, 0, SDL_OPENGL);

  /* did we get what we want? */
  if (!p_surface_window)
    {
      printf ("Couldn't open SDL-window: %s\n", SDL_GetError ());
      exit (-2);
    }

  /* Initialize projection.  */
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  glOrtho (0.0, 1.0, 1.0, 0.0, -1.0, 1.0);

  /* Initialize texture.  */
  texture_format = (SDL_BYTEORDER == SDL_BIG_ENDIAN
		    ? GL_UNSIGNED_INT_8_8_8_8_REV
		    : GL_UNSIGNED_BYTE);

  glGenTextures (1, &texture_id);
  glBindTexture (GL_TEXTURE_RECTANGLE_ARB, texture_id);
  glTexImage2D (GL_TEXTURE_RECTANGLE_ARB,
		0, GL_RGBA, WIN_WIDTH, WIN_HEIGHT, 0, GL_BGRA,
		texture_format, texture_data);

  return cairo_context;
}

void draw_gl (void)
{
  static GLint vertices[12] = {
    0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0};
  static GLint tex_coords[8] = {
    0, 0, WIN_WIDTH, 0, WIN_WIDTH, WIN_HEIGHT, 0, WIN_HEIGHT};
  static GLint indices[4] = {
    0, 1, 2, 3};

  /* Copy texture data.  */
  glTexSubImage2D (GL_TEXTURE_RECTANGLE_ARB, 0,
		   0, 0, WIN_WIDTH, WIN_HEIGHT, GL_BGRA,
		   texture_format, texture_data);

  /* Draw a textured rectangle.  */
  glEnable (GL_TEXTURE_RECTANGLE_ARB);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glVertexPointer(3, GL_INT, 0, vertices);
  glTexCoordPointer(2, GL_INT, 0, tex_coords);
  glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, indices);

  SDL_GL_SwapBuffers ();
}

void
free_gl (void)
{
  glDeleteTextures (1, &texture_id);
  SDL_Quit ();
}
