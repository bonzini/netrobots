#include <stdio.h>
#include <stdbool.h>
#include "net_commands.h"

cmd_t cmds[] = {
	{ "CYCLE", NULL, 0, true },
	{ "CANNON", NULL, 2, true },
	{ "SCAN", NULL, 2, true },
	{ "LOC_X", NULL, 1, false },
	{ "LOC_Y", NULL, 1, false },
	{ "DAMAGE", NULL, 1, false },
	{ "SPEED", NULL, 1, false },
	{ NULL, NULL, 0, false }
};
