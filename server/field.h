/* Map drawing module interface.
   Paolo Bonzini, August 2008.

   This source code is released for free distribution under the terms
   of the GNU General Public License.  */

#ifndef MAP_H
#define MAP_H

#include <cairo.h>
#include <SDL.h>

/* Initialize the rendering of the map.  */
extern void init_map (void);

/* Render the map on every iteration.  */
extern void do_map (cairo_t *, SDL_Event *);

#endif /* MAP_H */
