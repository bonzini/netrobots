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
#include "robotserver.h"

static cairo_t *map_context;

/*
 * transforms degrees in to radians
 */
double
degtorad(int degrees)
{
	double radiants;
	radiants = degrees * M_PI/180;
	return radiants;
}

void
shot_animation(cairo_t *cr, double direction, struct cannon can)
{
	int time = can.timeToReload-150;/*reduce the reload time by half of it so it draws the
										explosion and the flash for half the reload time*/
	
	if(can.timeToReload < 1)return;/*if the gun is loaded don't paint anything*/
	
	cairo_save (cr);
	cairo_pattern_t *pat;

	/*flash of the shot*/
	cairo_rotate(cr, direction);
	
	pat = cairo_pattern_create_linear (128, 64,
                                     128, 128.0);
	cairo_pattern_add_color_stop_rgba (pat, 0, 1, 0, 0, time/150);
	cairo_pattern_add_color_stop_rgba (pat, 1, 1, 1, 0, time/150);
	cairo_set_source (cr, pat);
	
	cairo_move_to (cr, 32, 50);
	cairo_arc_negative (cr, 32, 32, 18, 90 * (M_PI/180.0), 0);
	cairo_arc_negative (cr, 68, 32, 18, M_PI, 90 * (M_PI/180.0));
	cairo_arc_negative (cr, 50, 50, 18, M_PI, 0);
	cairo_fill (cr);
	cairo_pattern_destroy (pat);
	
	/* explosion*/
	cairo_arc (cr, can.x, can.y, 40, 0, 2*M_PI);
	pat = cairo_pattern_create_radial (can.x, can.y,
                                     10, can.x, can.y, 40);
	cairo_pattern_add_color_stop_rgba (pat, 0, 1, 0, 0, time/150);
	cairo_pattern_add_color_stop_rgba (pat, 0.3, 1, 0.5, 0, time/150);
	cairo_pattern_add_color_stop_rgba (pat, 0.6, 1, 0.2, 0, time/150);
	cairo_set_source (cr, pat);
	cairo_fill (cr);
	cairo_pattern_destroy (pat);
	
	cairo_restore(cr);

}

/*
 *draws the cannon with the right orientation
 */
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

/*
 *draws the radar inside the robot with the right orientation
 */
void
draw_radar(cairo_t *cr, double direction)
{
	double x1=0, y1=22,
		x2=20, y2=-10,
		x3=-20, y3=-10;
	
	cairo_save(cr);
	cairo_rotate(cr, direction);
	cairo_set_source_rgba(cr, 1, 0, 0, 0.8);
	cairo_move_to(cr, x1, y1);
	cairo_line_to(cr, x2, y2);
	cairo_line_to(cr, x3, y3);
	cairo_close_path(cr);
	
	cairo_fill(cr);
	cairo_restore(cr);
}

/*
 *draws the name and a health bar for every robot (maybe in the future also the reloading animation
 */
void
draw_stats(cairo_t *cr, struct robot **all)
{
	int i;
	int space = 50;
	cairo_pattern_t *pat;

	cairo_save (cr);
	
	for(i = 0; i < max_robots; i++){
		
		/* display the name of the robot*/
		cairo_select_font_face (cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                               CAIRO_FONT_WEIGHT_BOLD);
		cairo_set_font_size (cr, 13.0);

		cairo_move_to (cr, 10.0, 15.0+i*space);		
		cairo_show_text (cr, all[i]->name);
		
		/* line with gradient from red to green*/
		cairo_move_to (cr, 10, 25+i*space);
		cairo_set_line_width (cr, 30);
		cairo_line_to (cr, 110, 25+i*space);
		pat = cairo_pattern_create_linear (0.0, 0.0,  100.0, 100.0);
		cairo_pattern_add_color_stop_rgba (pat, 1, 1, 0, 0, 1);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0, 1, 0, 1);
		cairo_set_source (cr, pat);
		cairo_stroke (cr);
		cairo_pattern_destroy (pat);
		
		/*black line on top of the colored one*/
		cairo_move_to (cr, 10, 25+i*space);
		cairo_set_line_width (cr, 24);
		cairo_set_source_rgba (cr, 0, 0, 0, 0.5);
		cairo_line_to (cr, 110 - all[i]->damage, 25+i*space);
		cairo_stroke (cr);
		
		/*reloading animation goes here*/
		
		
		/*separation line between the different robots*/
		cairo_move_to (cr, 20, 45+i*space);
		cairo_set_line_width (cr, 4);
		cairo_set_source_rgb (cr, 0, 0, 0);
		cairo_line_to (cr, 100, 45+i*space);
		cairo_stroke (cr);
		
	}
	cairo_restore (cr);
}

/*
 *draws a robot with a given size, using the various parameters(orientation, position,..)
 *from the robot struct
 */
void
draw_robot(cairo_t *cr, struct robot *myRobot, double size)
{
	double x1=-70, y1=-30,
		y2=10,
		x4=70,
		x5=0, y5=-100;
	double px2=-70, py2=100,
		px3=70, py3=100,
		px4=70, py4=10;
	
	cairo_save(cr);
	
	cairo_translate(cr, myRobot->x, 1000 - myRobot->y);
	cairo_scale(cr, size, size);
	cairo_save(cr);
	cairo_rotate(cr, degtorad(myRobot->degree));
	
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
	draw_cannon(cr, degtorad(myRobot->cannon_degree));
	draw_radar(cr, degtorad(myRobot->radar_degree));
	shot_animation(cr, degtorad(myRobot->cannon_degree), myRobot->cannon[0]);
	shot_animation(cr, degtorad(myRobot->cannon_degree), myRobot->cannon[1]);
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
	int i;
	cairo_save (cr);
  
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_paint(cr);
	cairo_scale(cr, WIN_HEIGHT/1000.0, WIN_HEIGHT/1000.0);
	for(i = 0; i < max_robots; i++){
		draw_robot(cr, all_robots[i], 0.5);
	}
	cairo_restore(cr);	
/* cairo_save (cairo_context);
  cairo_set_operator (cairo_context, CAIRO_OPERATOR_SOURCE);
  cairo_set_source_surface (cairo_context, cairo_get_target (map_context), 0, 0);
  cairo_paint (cairo_context);
  cairo_restore (cairo_context);*/
}

