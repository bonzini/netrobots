#include <stdio.h>
#include <stdbool.h>
#include "net_defines.h"

cmd_t cmds[] = {
	{ NULL, 0, true }, // CYCLE
	{ NULL, 2, true }, // CANNON
	{ NULL, 2, true }, // SCAN
	{ NULL, 1, false }, // LOC_X
	{ NULL, 1, false }, // LOC_Y
	{ NULL, 1, false }, // DAMAGE
	{ NULL, 1, false }, // SPEED
	{ NULL, 0, false }
};
