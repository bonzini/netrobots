/* Map drawing module interface.
   Paolo Bonzini, August 2008.

   This source code is released for free distribution under the terms
   of the GNU General Public License.  */

#ifndef MAP_H
#define MAP_H

#include <cairo.h>
#include <SDL.h>

/* Initialize the rendering of the map.  */
extern void init_cairo ();

/* Update the canvas */
extern void update_display(SDL_Event *);

/* Updates the Canvas */
extern void update_display(SDL_Event *);


#endif /* MAP_H */
