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
	{ cmd_loc_x, 1, false }, // LOC_X
	{ cmd_loc_y, 1, false }, // LOC_Y
	{ cmd_damage, 1, false }, // DAMAGE
	{ cmd_speed, 1, false }, // SPEED
	{ cmd_drive, 2, true }, // MOVE
};

result_t error_res = { -1, true, false };

int cmdn = sizeof(cmds)/sizeof(cmd_t);


int
cmd_cycle (struct robot *robot, int *args)
{
//	cycle(robot);
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
