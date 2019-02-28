//This file contains specific autonomous routiens

// Auton to run in square closest to flag
int autonTime=0;

void nearAutonFirstHalf(int side, bool flipCap){
	startTask(clawTask);
	clawPID.target=clawClosePos;
	startTask(liftControl);

	motor[intake]=127;
	if(flipCap) pDrive(-1100);//back up to get first ball
	else pDrive(-960);
	motor[intake]=0;
	if(flipCap) pDrive(1180);
	else pDrive(940);

	SensorValue[gyro]=0;
	motor[slingshot]=127;
	pTurn(870*side);//turn to align with flag
	motor[slingshot] = 0;
	motor[intake]=127;
	wait1msec(400);
	motor[slingshot] = 127;
	pDrive(-600);//align with second flag
	wait1Msec(150);//shoot second fllag
	motor[slingshot]=0;
	pTurn(130*side,false);//align with bottom flag


}

void nearAuton(int side){
	clearTimer(T1);
	nearAutonFirstHalf(side, false);
	pDrive(-600);//hit bottom flag
	pTurn(-80*side,false);
	pDrive(1800);
	pTurn(900*side);

	drive(127);
	wait1Msec(1400);
	drive(0);



	autonTime=time1[T1];

}

void prog(){
	clearTimer(T1);
	nearAutonFirstHalf(BLUESIDE, true);
	SensorValue[gyro]=0;
	pDrive(-600);//hit bottom flag
	pDrive(700);
	clawPID.target=clawOpenPos;
	pTurn(-900,false);
	pDrive(470);
	clawPID.target=clawClosePos;//grab cap
	wait1Msec(200);
	pTurn(900,false);
	pDrive(300);
	liftPID.target+=300;
	startTask(rotatorTask);
	rotatorPID.target=rotatorHighPos;//flip cap
	wait1Msec(500);
	liftPID.target-=300;
	clawPID.target=clawOpenPos;
	pDrive(-300);
	clawPID.target=clawClosePos;
	clawIdle = true;
	pTurn(-900,false);
	pDrive(670);
	pTurn(900,false);
	drive(-127);
	wait1Msec(1000);
	drive(0);
	motor[intake]=127;
	pDrive(750);
	motor[slingshot]=127;
	pTurn(150);
	wait1Msec(300);
	motor[slingshot]=0;


	//pTurn(900);
	//pDrive(1000);

	autonTime=time1[T1];
}


void farAutonFirstHalf(int side){
//red is 1, blue is -1
	clearTimer(T1);

	motor[slingshot] = 127;
	pTurn(580 * side);
	wait1Msec(300);
	motor[slingshot] = 0;
	pTurn(-580 * side);
	clawIdle = false;
	startTask(clawTask);
	startTask(liftControl);
	clawPID.target=clawOpenPos+220;
	startTask(rotatorTask);
	rotatorPID.target=rotatorLowPos;
	motor[intake] = 127;
	pDrive(-900);//back up to get first ball
	motor[intake]=0;
	liftPID.target=550;//lower lift
	//pDrive(-270);
	pDrive(130);
	pTurn(1000);
	pDrive(400);
	clawPID.target=clawClosePos;//grab cap
	liftPID.target+=300;
	rotatorPID.target=rotatorHighPos;//flip cap
	wait1Msec(500);
	liftPID.target-=300;
	motor[intake] = 127;
	clawPID.target=clawOpenPos;
	getLiftOutOfTheWay();
	liftPID.target+=100;
}

// Auton to run in sqaure farthest to flag
void farAutonShoot(int side){
//red is 1, blue is -1
	farAutonFirstHalf(side);
	pDrive(-550);//back up to align on bar
	motor[intake]=0;
	drive(-50);
	wait1Msec(300);
	pDrive(150);
	motor[slingshot]=127;
	pTurn(-170);
	wait1Msec(300);

	motor[slingshot]=0;


	/**
	clawIdle = false;
	rotatorPID.target=rotatorLowPos;
	startTask(clawTask);
	startTask(liftControl);
	liftPID.target = 700;//put lift on ground for claw to grab cap
	getLiftOutOfTheWay();
	clawPID.target=clawClosePos;
	startTask(rotatorTask);
	rotatorPID.target=rotatorLowPos;
	//put lift on ground for claw to grab cap
	motor[intake] = 127;
	pDrive(-1140);//back up to hit cap
	pDrive(160);

	pTurn(950 * side);//turn to face second cap

	clawPID.target=clawOpenPos+140;
	startTask(rotatorTask);
	rotatorPID.target = rotatorLowPos;
	motor[intake] = 0;
	liftPID.target=400;//put lift on ground
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
	**/



	//*/
	autonTime=time1[T1];
}

void farAutonPark(int side){
	farAutonFirstHalf(side);
	pDrive(-550);//back up to align on bar
	motor[intake]=0;
	drive(-50);
	wait1Msec(300);
	pDrive(150);

	clawPID.target=clawClosePos;
	liftPID.target+=200;
	pTurn(1900);

	drive(40);
	wait1Msec(500);
	drive(127);
	wait1Msec(900);
	drive(-127);
	wait1Msec(100);
	drive(0);
}
