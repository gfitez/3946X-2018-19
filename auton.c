//This file contains specific autonomous routiens

// Auton to run in square closest to flag
int autonTime=0;
void nearAutonFirstHalf(int side, bool flipCap){
	startTask(clawTask);
	clawPID.target=clawClosePos;
	startTask(liftControl);
	liftPID.target=liftOutOfTheWayHeight;
	if(flipCap)liftPID.target-=100;

	motor[intake]=127;
	if(flipCap) pDrive(-1350);//back up to get first ball
	else pDrive(-920);
	motor[intake]=0;
	if(flipCap) pDrive(1390);
	else pDrive(900);

	SensorValue[gyro]=0;
	motor[slingshot]=127;
	pTurn(850*side);//turn to align with flag
	motor[slingshot] = 0;
	motor[intake]=127;
	wait1msec(400);
	motor[slingshot] = 127;
	pDrive(-600);//align with second flag
	wait1Msec(150);//shoot second fllag
	motor[slingshot]=0;
	if(side==BLUESIDE)pTurn(170*side);
	else pTurn(170*side);//align with bottom flag


}

void nearAuton(int side){
	clearTimer(T1);
	nearAutonFirstHalf(side, false);
	pDrive(-600);//hit bottom flag
	pTurn(-150*side,false);
	pDrive(1900);
	pTurn(800*side);

	drive(127);
	wait1Msec(1400);
	drive(0);



	autonTime=time1[T1];

}

void prog(){
	clearTimer(T1);
	nearAutonFirstHalf(REDSIDE, true);
	liftPID.target+=100;
	SensorValue[gyro]=0;
	pDrive(-600);//hit bottom flag
	pTurn(-200);
	liftPID.target=500;
	pDrive(630);
	pTurn(900, false);
	drive(-127);
	wait1Msec(500);
	drive(0);
	SensorValue[gyro] = 0;
	motor[intake]=0;
	clawPID.target=clawOpenPos-200;
	pDrive(570);
	clawPID.target=clawClosePos;//grab cap
	wait1Msec(200);
	liftPID.target+=300;
	pTurn(-900,false);



	startTask(rotatorTask);
	rotatorPID.target=rotatorHighPos;//flip cap
	wait1Msec(500);
	liftPID.target-=300;
	clawPID.target=clawOpenPos;
	pDrive(-200);

	clawPID.target=clawClosePos;
	wait1Msec(250);
	pTurn(900,false);
	clawIdle = true;
	pDrive(815);
	pTurn(-900,false);
	liftPID.target+=100;//lift to avoid burning out intake
	motor[intake]=127;
	drive(-127);//back up to align on wall
	wait1Msec(1000);
	drive(0);
	SensorValue[gyro]=0;//line up on wall

	pDrive(700);
	liftPID.target=liftOutOfTheWayHeight;
	motor[intake]=127;
	motor[slingshot]=127;//shoot middle flag
	pTurn(-240);
	wait1Msec(500);
	motor[slingshot]=0;
	motor[intake]=0;
	pTurn(995);//line up with next cap
	clawIdle=false;
	liftPID.target=500;
	clawPID.target=clawOpenPos;
	pDrive(1000);
	clawPID.target=clawClosePos;//grab cap
	wait1Msec(500);
	liftPID.target+=400;
	pTurn(-900);
	rotatorPID.target=rotatorLowPos;//flip third cap
	wait1Msec(750);
	clawPID.target=clawOpenPos-200;
	liftPID.target-=400;
	pDrive(-200);
	pTurn(900);
	pDrive(150);
	pTurn(-900);
	drive(-127);//back up to hit third bottom flag
	wait1Msec(1000);
	drive(0);
	SensorValue[gyro]=0;

	//get to platform and park
	clawPID.target=clawClosePos;

	pDrive(500);
	pTurn(900);
	pDrive(300);
	pTurn(-900);
	pDrive(1600);
	liftPID.target=700;
	wait1Msec(200);
	pTurn(-900);//turn to align with park


	//get on lower platform
	drive(50);
	wait1Msec(1000);
	drive(127);
	wait1Msec(1100);

	//get on higher platform
	drive(30);
	wait1Msec(500);
	drive(127);
	wait1Msec(1000);
	drive(-127);
	wait1Msec(100);
	drive(0);
	//pTurn(900);
	//pDrive(1000);

	autonTime=time1[T1];
}


void farAutonFirstHalf(int side){
//red is 1, blue is -1
	clearTimer(T1);

	motor[slingshot] = 127;
	startTask(liftControl);
	if(side==BLUESIDE)pTurn(610*side);
	else pTurn(580 * side);
	wait1Msec(300);
	motor[slingshot] = 0;
	if(side==BLUESIDE)pTurn(-610*side);
	else pTurn(-580 * side);
	clawIdle = false;
	startTask(clawTask);

	clawPID.target=clawOpenPos+220;
	startTask(rotatorTask);
	rotatorPID.target=rotatorLowPos;
	motor[intake] = 127;
	pDrive(-900);//back up to get first ball
	motor[intake]=0;
	liftPID.target=550;//lower lift
	//pDrive(-270);
	pDrive(130);
	pTurn(1000*side);
	pDrive(380);
	clawPID.target=clawClosePos;//grab cap
	liftPID.target+=300;
	rotatorPID.target=rotatorHighPos;//flip cap
	wait1Msec(500);
	liftPID.target-=300;
	if(side==BLUESIDE)pTurn(-160*side);
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
	wait1Msec(600);
	SensorValue[gyro]=0;
	pDrive(150);
	motor[slingshot]=127;
	if(side==BLUESIDE)pTurn(-130*side);
	else pTurn(-190*side);
	wait1Msec(500);
	pDrive(100);

	motor[slingshot]=0;
	drive(-127);
	wait1Msec(1500);
	drive(127);
	wait1Msec(150);
	drive(0)

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
