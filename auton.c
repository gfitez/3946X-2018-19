//This file contains specific autonomous routiens

// Auton to run in square closest to flag
int autonTime=0;

void nearAutonFirstHalf(int side){
	motor[intake]=127;
	motor[slingshot]=127;
	clearTimer(T1);
	startTask(clawTask);
	clawPID.target=clawClosePos;
	startTask(liftControl);
	liftPID.target = liftOutOfTheWayHeight+50;

	wait1Msec(2500);
	motor[slingshot]=0;


	pDrive(-670-450);//Drive back to hit flag

	//turns claw towards ground

	pDrive(570);//drive back
	clawPID.target=clawOpenPos+130;
	liftPID.target = 550;//put lift on ground for claw to grab cap
	motor[intake]=0;


	pTurn(900 * side);//turn to face cap



	startTask(rotatorTask);
	rotatorPID.target = rotatorLowPos;


	pDrive(740);
	clawPID.target=clawClosePos;//grap capa
	liftPID.target+=200;//lift for cap flip
	wait1Msec(200);
	rotatorPID.target=rotatorHighPos;
	liftPID.target=850;//lift up
	wait1Msec(700);
	drive(127);
	clawPID.target=clawOpenPos;
	liftPID.target-=200;//lower for cap flip
	wait1msec(200);
	drive(0);

	//pDrive(-0);//Drive back to align with platform
}

void nearAuton(int side){

	nearAutonFirstHalf(side);
//park below

	pTurn(-900*side);//turn to face platform
	liftPID.target+=200;//make sure claw does not hit on park
	clawPID.target=clawClosePos;
	pDrive(875);
	drive(127);
	wait1Msec(500);
	liftPID.target-=200;
	wait1msec(520);

	drive(-100);
	wait1Msec(100);
	drive(0);
	//*/


	autonTime=time1[T1];

}

void prog(){
	nearAutonFirstHalf(REDSIDE);
	pDrive(-500);
	clawPID.target=clawClosePos;
	pTurn(-930);
	pDrive(1310);
	pTurn(900);


	drive(127);
	wait1msec(1200);
	drive(30);
	wait1Msec(1000);
	drive(127);
	wait1Msec(850);
	drive(-127);
	wait1Msec(100);
	drive(0);
}




// Auton to run in sqaure farthest to flag
void farAuton(int side){
//red is 1, blue is -1
	clearTimer(T1);
	clawIdle = false;
	rotatorPID.target=rotatorLowPos;
	startTask(clawTask);
	startTask(liftControl);
	liftPID.target = 400;//put lift on ground for claw to grab cap
	getLiftOutOfTheWay();
	clawPID.target=clawClosePos;
	startTask(rotatorTask);
	rotatorPID.target=rotatorLowPos;
	//put lift on ground for claw to grab cap
	motor[intake] = 127;
	pDrive(-1140);//back up to hit cap
	pDrive(160);

	pTurn(950 * side);//turn to face second cap
	motor[intake] = 0;
	clawPID.target=clawOpenPos+140;
	startTask(rotatorTask);
	rotatorPID.target = rotatorLowPos;
	pDrive(400);
	clawPID.target=clawClosePos;//grap capa
	wait1Msec(200);
	rotatorPID.target=rotatorHighPos;
	liftPID.target=850;
	wait1Msec(500);
	clawPID.target=clawOpenPos;
	liftPID.target-=50;
	pDrive(-230);
	pTurn(-900*side);
	pDrive(200);
	//comment out below for no park

	pTurn(-900*side);//tur to face park

	clawPID.target=clawClosePos;

	drive(127);
	wait1msec(1325);
	drive(-127);
	wait1Msec(100);
	drive(0);
	stopTask(clawTask);




	//*/
	autonTime=time1[T1];
}
