#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

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
execute_cmd (char *input)
{
	char **argv;
	int argc;
	cmd_t cmd;
	result_t res;

	argc = str_to_argv(input, &argv);
	if (!argc || !str_isnumber(argv[0]) || atoi(argv[0]) < 0 || atoi(argv[0]) >= cmdn)
		return error_res;
	cmd = cmds[atoi(argv[0])];
	if (cmd.args != argc - 1)
		return error_res;

	res.cycle = cmd.cycle;
	res.error = false;
// 	res.result = cmd.func();

	return res;
}
