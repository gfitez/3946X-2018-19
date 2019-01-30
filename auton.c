//This file contains specific autonomous routiens

// Auton to run in square closest to flag
int autonTime=0;
void nearAuton(int side){
	motor[intake]=127;
	motor[slingshot]=127;
	pDrive(-450);
	clearTimer(T1);
	startTask(clawTask);
	clawPID.target=clawClosePos;
	startTask(liftControl);
	liftPID.target = 850;

	wait1Msec(2000);
	motor[slingshot]=0;


	pDrive(-600);//Drive back to hit flag

	//turns claw towards ground

	pDrive(600);//drive back
	liftPID.target = 500;//put lift on ground for claw to grab cap
	motor[intake]=0;


	pTurn(900 * side);//turn to face cap
	clawPID.target=clawOpenPos+100;


	startTask(rotatorTask);
	rotatorPID.target = rotatorLowPos;


	pDrive(750);
	clawPID.target=clawClosePos;//grap capa

	rotatorPID.target=rotatorHighPos;
	liftPID.target=850;//lift up
	wait1Msec(600);
	drive(127);
	clawPID.target=800;
	wait1msec(200);
	drive(0);

	//pDrive(-0);//Drive back to align with platform


	pTurn(-900*side);//turn to face platform
	clawPID.target=clawClosePos;
	drive(127);
	wait1msec(2000);
	drive(-50);
	wait1Msec(50);
	drive(0);



	autonTime=time1[T1];

}
// Auton to run in sqaure farthest to flag
void farAuton(int side){
//red is 1, blue is -1
	clearTimer(T1);
	rotatorPID.target=rotatorLowPos;
	startTask(clawTask);
	startTask(liftControl)

	clawPID.target=50;
	wait1Msec(500);

	liftPID.target=950;
	wait1Msec(500);

	startTask(rotatorTask);
	rotatorPID.target=rotatorLowPos;
	liftPID.target = 450;//put lift on ground for claw to grab cap
	drive(127);
	wait1Msec(1225);
	drive(0);
	pDrive(-200);
	clawPID.target=670;
	pTurn(-220*side);//turn to face second cap
	pDrive(520);


	clawPID.target=50;//grab second cap
	rotatorPID.target=rotatorHighPos//flip cap
	liftPID.target=1000;//lift up
	wait1Msec(600);
	liftPID.target=500;
	clawPID.target=800;//drop cap

	pTurn(520*side);//turn to face platform



	drive(127);
	wait1msec(1750);
	drive(-50);
	wait1Msec(50);
	drive(0);


	autonTime=time1[T1];
}
