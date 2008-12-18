int
testScanAndShoot()
{
	struct robot walle = {
		"Wall-E",
		true,
		100, 100,
		0, 0, 0, 0, 0, 0, 0,
		{
			{ 0, 0, 0 },
			{ 0, 0, 0 }	
		}
	};
	
	struct robot eve = {
		"Eve",
		true,
		500, 500,
		0, 0, 0, 0, 0, 0, 0,
		{
			{ 0, 0, 0 },
			{ 0, 0, 0 }			
		}
	};
	
	struct robot masiar = {
		"Bubi",
		true,
		300, 300,
		0, 0, 0, 0, 0, 0, 0,
		{
			{ 0, 0, 0 },
			{ 0, 0, 0 }			
		}
	};
	
	struct robot *robogang[3];
	robogang[0] = &walle;
	robogang[1] = &eve;
	robogang[2] = &masiar;
	
	all_robots = robogang;
	max_robots = 3;
	
	int distance = scan(&walle, 315, 10);
	printf("Closest robot is at %dm from Walle\n", distance);
	
	int result = cannon(&walle, 315, distance);
	printf("Walle shots the closest robot! Its damage is now %d\n", masiar.damage);	
	
	result = cannon(&walle, 315, distance - 10);
	printf("Walle shots again at the closest robot! Its damage is now %d\n", masiar.damage);
	
	
	result = cannon(&walle, 315, distance - 20);
	printf("Walle shots again at the closest robot! Its damage is still %d\n", masiar.damage);
	
	int i;
	for(i = 0; i < 320; i++)
		cycle();
	
	result = cannon(&walle, 315, distance - 20);
	printf("Walle shots again at the closest robot! Its damage is now %d\n", masiar.damage);
	
	distance = scan(&walle, 305, 9);
	printf("Walle seems to be alone. The scan returns %dm\n", distance);
}

int
testDriveAndCycle ()
{
	struct robot runner = {
		"Forrest Gump",
		true,
		100, 100,
		0, 0, 0, 0, 0, 0, 0,
		{
			{ 0, 0, 0 },
			{ 0, 0, 0 }	
		}
	};
	
	struct robot *robogang[1];
	robogang[0] = &runner;
	
	all_robots = robogang;
	max_robots = 1;
	
	printf("Before start driving x=%g, y=%g\n", runner.x, runner.y);
	drive(&runner, 270, 100);
	int i;
	for(i = 0; i < 50; i++){
		cycle();
		printf("After %d cycle(s) x=%g, y=%g BD %d\n", i+1, runner.x, runner.y, runner.break_distance);
	}
	drive(&runner, 40, 0);
	for(i = 0; i < 40; i++){
		cycle();
		printf("After %d cycle(s) x=%g, y=%g BD %d\n", i+1, runner.x, runner.y, runner.break_distance);
	}
	
}

int
testRobotsCollision ()
{
	struct robot rock = {
		"I'm rock!",
		true,
		100, 12,
		0, 0, 0, 0, 0, 0, 0,
		{
			{ 0, 0, 0 },
			{ 0, 0, 0 }	
		}
	};
	
	struct robot stone = {
		"I'm stone!",
		true,
		100, 100,
		0, 0, 0, 0, 0, 0, 0,
		{
			{ 0, 0, 0 },
			{ 0, 0, 0 }	
		}
	};
	
	struct robot *robogang[2];
	robogang[0] = &stone;
	robogang[1] = &rock;
	
	all_robots = robogang;
	max_robots = 2;
	
	drive(&stone, 90, 100);
	
	int i;
	for(i = 0; i < 100; i++){
		cycle();
		printf("x1=%g, y1=%g x2=%g, y2=%g \n", rock.x, rock.y, stone.x, stone.y);
		
	}
	printf("Damage of robot 1 %d damage of robot 2 %d\n", rock.damage, stone.damage);
}

int
main(){
	testScanAndShoot();
	testDriveAndCycle();
	testRobotsCollision();
}