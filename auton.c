//This file contains specific autonomous routiens

// Auton to run in square closest to flag
int autonTime=0;

void nearAutonFirstHalf(int side){
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
	clawPID.target=clawOpenPos+30;


	startTask(rotatorTask);
	rotatorPID.target = rotatorLowPos;


	pDrive(700);
	clawPID.target=clawClosePos;//grap capa
	wait1Msec(200);
	rotatorPID.target=rotatorHighPos;
	liftPID.target=850;//lift up
	wait1Msec(600);
	drive(127);
	clawPID.target=clawOpenPos;
	wait1msec(200);
	drive(0);

	//pDrive(-0);//Drive back to align with platform
}

void nearAuton(int side){

	nearAutonFirstHalf(side);


	pTurn(-900*side);//turn to face platform
	clawPID.target=clawClosePos;
	drive(127);
	wait1msec(1850);
	drive(-50);
	wait1Msec(50);
	drive(0);



	autonTime=time1[T1];

}

void prog(){
	nearAutonFirstHalf(BLUESIDE);
	pDrive(-500);
	pTurn(930);
	pDrive(1200);
	pTurn(-900);

	drive(127);
	wait1msec(2300);
	drive(-50);
	wait1Msec(50);
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
	getLiftOutOfTheWay();
	clawPID.target=clawClosePos;
	startTask(rotatorTask);
	rotatorPID.target=rotatorLowPos;
	//put lift on ground for claw to grab cap
	motor[intake] = 127;
	pDrive(-1300);//back up to hit cap
	pDrive(230);
	motor[intake] = 0;
	pTurn(900 * side);//turn to face second cap
	liftPID.target = 400;//put lift on ground for claw to grab cap
	clawPID.target=clawOpenPos+60;
	startTask(rotatorTask);
	rotatorPID.target = rotatorLowPos;
	pDrive(420);
	clawPID.target=clawClosePos;//grap capa
	wait1Msec(200);
	rotatorPID.target=rotatorHighPos;
	liftPID.target=850;
	wait1Msec(200);
	clawPID.target=clawOpenPos;
	pDrive(-200);
	pTurn(-900*side);
	pDrive(350);
	pTurn(-900*side);//tur to face park

	clawPID.target=clawClosePos;
	drive(127);
	wait1msec(1250);
	drive(-127);
	wait1Msec(50);
	drive(0);


	//lift up
	/*
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
	drive(0);*/


	autonTime=time1[T1];
}
