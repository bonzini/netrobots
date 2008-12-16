/* Animation module.
   Paolo Bonzini, August 2008.

   This source code is released for free distribution under the terms
   of the GNU General Public License.  */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include "anim.h"
#include "drawing.h"

struct time cur_time;
static int speed = 1;
static int set_to_localtime;

void
init_anim (void)
{
 
}

void
do_anim (cairo_t *cairo_context, SDL_Event *event)
{
  if (event->type == SDL_KEYDOWN)
    {
      if (event->key.keysym.sym == SDLK_q)
        {
          event->type = SDL_QUIT;
          SDL_PushEvent (event);
        }
    }
}
