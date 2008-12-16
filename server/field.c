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

void
draw_cannon(cairo_t *cr, double direction)
{
	double x1=-5, y1=51,
		x2=-5, y2=19,
		x3=5, y3=19,
		x4=5, y4=51;
		
	cairo_save(cr);
	cairo_rotate(cr, direction);
	cairo_set_source_rgba (cr, 0, 0, 0, 1);	
	cairo_move_to (cr, x1, y1);
	cairo_line_to (cr, x2, y2);
	cairo_line_to (cr, x3, y3);
	cairo_line_to (cr, x4, y4);
	cairo_close_path(cr);
	
	cairo_fill(cr);
	cairo_restore(cr);
}

void
draw_radar(cairo_t *cr, double direction)
{
	double x1=0, y1=22,
		x2=20, y2=-10,
		x3=-20, y3=-10;
	
	cairo_save(cr);
	cairo_rotate(cr, direction);
	cairo_set_source_rgba(cr, 0, 1, 0, 0.8);
	cairo_move_to(cr, x1, y1);
	cairo_line_to(cr, x2, y2);
	cairo_line_to(cr, x3, y3);
	cairo_close_path(cr);
	
	cairo_fill(cr);
	cairo_restore(cr);
}

void
draw_robot(cairo_t *cr, double x, double y, double size, double rotation)
{
	double x1=-70, y1=-30,
		y2=10,
		x4=70,
		x5=0, y5=-100;
	double px2=-70, py2=70,
		px3=70, py3=70,
		px4=70, py4=10;
	
	cairo_save(cr);
	
	cairo_translate(cr, x, y);
	cairo_scale(cr, size, size);
	cairo_save(cr);
	cairo_rotate(cr, rotation);
	
	cairo_set_source_rgba (cr, 0.2, 0.4, 0.5, 0.6);
	cairo_set_line_width (cr, 2);
	cairo_move_to (cr,x1,y1);   
	cairo_line_to (cr,x1,y2);
	cairo_curve_to (cr, px2, py2, px3, py3, px4, py4);
	cairo_line_to (cr, x4, y1);
	cairo_line_to (cr, x5, y5);
	cairo_close_path (cr);
	cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
	cairo_move_to (cr, 30, 0);
	cairo_arc_negative (cr, 0, 0, 30, 0, -2*M_PI);

	cairo_fill_preserve (cr);
	cairo_set_source_rgba (cr, 0.1, 0.7, 0.5, 0.5);	
	
	cairo_stroke (cr);
	cairo_restore(cr);
	draw_cannon(cr, 3.14);
	draw_radar(cr, 4.71);
	cairo_restore(cr);
}


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
  
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);

	draw_robot(cr, 200, 200, 0.5, 1);
	cairo_restore(cr);	
/*
  cairo_save (cairo_context);
  cairo_set_operator (cairo_context, CAIRO_OPERATOR_SOURCE);
  cairo_set_source_surface (cairo_context, cairo_get_target (map_context), 0, 0);
  cairo_paint (cairo_context);
  cairo_restore (cairo_context);

  render_map_marching_squares (cairo_context, civil_rise_set, 0.25);
  render_map_marching_squares (cairo_context, sun_rise_set, 0.33);*/
}

