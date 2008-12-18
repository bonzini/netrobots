#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "robotserver.h"
#include "net_utils.h"
#include "net_defines.h"

int cmd_cycle (struct robot *robot, int *args);
int cmd_cannon (struct robot *robot, int *args);
int cmd_scan (struct robot *robot, int *args);
int cmd_loc_x (struct robot *robot, int *args);
int cmd_loc_y (struct robot *robot, int *args);
int cmd_damage (struct robot *robot, int *args);
int cmd_speed (struct robot *robot, int *args);
int cmd_drive (struct robot *robot, int *args);

cmd_t cmds[] = {
	{ cmd_cycle, 0, true }, // CYCLE
	{ cmd_cannon, 2, true }, // CANNON
	{ cmd_scan, 2, true }, // SCAN
	{ cmd_loc_x, 0, false }, // LOC_X
	{ cmd_loc_y, 0, false }, // LOC_Y
	{ cmd_damage, 0, false }, // DAMAGE
	{ cmd_speed, 0, false }, // SPEED
	{ cmd_drive, 2, true }, // MOVE
};

result_t error_res = { -1, true, false };

int cmdn = sizeof(cmds)/sizeof(cmd_t);


int
cmd_cycle (struct robot *robot, int *args)
{
	return 1;
}

int 
cmd_scan (struct robot *robot, int *args)   
{
	return scan(robot, args[0], args[1]);
}

int 
cmd_cannon (struct robot *robot, int *args)   
{
	return cannon(robot, args[0], args[1]);
}

int 
cmd_loc_x (struct robot *robot, int *args)   
{
	return loc_x(robot);
}

int 
cmd_loc_y (struct robot *robot, int *args)   
{
	return loc_y(robot);
}

int 
cmd_damage (struct robot *robot, int *args)   
{
	return damage(robot);
}

int 
cmd_speed (struct robot *robot, int *args)   
{
	return speed(robot);
}

int
cmd_drive (struct robot *robot, int *args)
{
	drive(robot, args[0], args[1]);
	return 1;
}

result_t
execute_cmd (struct robot *robot, char *input)
{
	char **argv;
	int argc, ret, *args, i;
	cmd_t cmd;
	result_t res;

	argc = str_to_argv(input, &argv);
	if (argc == -1)
		return error_res;
	if (!argc || !str_isnumber(argv[0])) {
		free(argv);
		return error_res;
	}

	i = atoi(argv[0]);
	if (i < 0 || i >= cmdn)
		return error_res;
	cmd = cmds[i];

	if (cmd.args != argc - 1) {
		free(argv);
		return error_res;
	}

	if (!(args = (int *) malloc(cmd.args * sizeof(int)))) {
		free(argv);
		return error_res;
	}

	for (i = 1; i < argc; i++) {
		if (!str_isnumber(argv[i]))
			return error_res;
		args[i - 1] = atoi(argv[i]);
	}


	ret = cmd.func(robot, args);
	ndprintf(stdout, "[COMMAND] %s -> %d recived - %g %g %d\n", argv[0], ret, robot->x, robot->y, robot->damage);
	if (ret == -1)
		return error_res;

	res.result = ret;
	res.cycle = cmd.cycle;
	res.error = false;
 	
	free(argv);

	return res;
}
