#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "robotserver.h"
#include "net_utils.h"
#include "net_defines.h"

cmd_t cmds[] = {
	{ NULL, 0, true }, // CYCLE
	{ NULL, 2, true }, // CANNON
	{ NULL, 2, true }, // SCAN
	{ NULL, 1, false }, // LOC_X
	{ NULL, 1, false }, // LOC_Y
	{ NULL, 1, false }, // DAMAGE
	{ NULL, 1, false }, // SPEED
};

result_t error_res = { -1, true, false };

int cmdn = sizeof(cmds)/sizeof(cmd_t);

result_t
execute_cmd (struct robot *robot, char *input)
{
	char **argv;
	int argc, ret, *args, i;
	cmd_t cmd;
	result_t res;

	argc = str_to_argv(input, &argv);
	if (!argc || !str_isnumber(argv[0]) || atoi(argv[0]) < 0 || atoi(argv[0]) >= cmdn)
		return error_res;
	cmd = cmds[atoi(argv[0])];
	if (cmd.args != argc - 1)
		return error_res;

	if (!(args = (int *) malloc(cmd.args * sizeof(int))))
		return error_res;
	for (i = 1; i < argc; i++) {
		if (!str_isnumber(argv[i]))
			return error_res;
		args[i - 1] = atoi(argv[i]);
	}

	ret = cmd.func(robot, args);
	if (ret == -1)
		return error_res;

	res.result = ret;
	res.cycle = cmd.cycle;
	res.error = false;
 	
	return res;
}
