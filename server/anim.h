/* Animation module interface.
   Paolo Bonzini, August 2008.

   This source code is released for free distribution under the terms
   of the GNU General Public License.  */

#ifndef ANIM_H
#define ANIM_H

#include <cairo.h>
#include <SDL.h>

struct time
{
  int year, month, day, h, m;
};

extern struct time cur_time;

extern void init_anim ();
extern void do_anim (cairo_t *cairo_context, SDL_Event *event);

#endif /* ANIM_H */
