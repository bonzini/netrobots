/* Map drawing module.
   Paolo Bonzini, August 2008.

   This source code is released for free distribution under the terms
   of the GNU General Public License.  */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include "drawing.h"
#include "field.h"
#include "anim.h"

static cairo_t *map_context;

/* can be used to add a background to the window*/
void
init_map (void)
{
  cairo_surface_t *png_map = cairo_image_surface_create_from_png ("background.png");
  map_context = create_cairo_context ();
  cairo_set_source_surface (map_context, png_map, 0, 0);
  cairo_paint (map_context);
  cairo_surface_destroy (png_map);
}

void
do_map (cairo_t *cr, SDL_Event *event)
{
  cairo_save (cr);
	double x=25.6,  y=128.0;
  double x1=102.4, y1=230.4,
    x2=153.6, y2=25.6,
    x3=230.4, y3=128.0;
  
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_move_to (cr, x, y);
  cairo_curve_to (cr, x1, y1, x2, y2, x3, y3);
  
  cairo_set_line_width (cr, 10.0);
  cairo_stroke (cr);
  
  cairo_set_source_rgba (cr, 1, 0.2, 0.2, 0.6);
  cairo_set_line_width (cr, 6.0);
  cairo_move_to (cr,x,y);   cairo_line_to (cr,x1,y1);
  cairo_move_to (cr,x2,y2); cairo_line_to (cr,x3,y3);
  cairo_stroke (cr);
  cairo_restore (cr);

/*
  cairo_save (cairo_context);
  cairo_set_operator (cairo_context, CAIRO_OPERATOR_SOURCE);
  cairo_set_source_surface (cairo_context, cairo_get_target (map_context), 0, 0);
  cairo_paint (cairo_context);
  cairo_restore (cairo_context);

  render_map_marching_squares (cairo_context, civil_rise_set, 0.25);
  render_map_marching_squares (cairo_context, sun_rise_set, 0.33);*/
}
