	#pragma config(Sensor, in1,    leftLift,       sensorPotentiometer)
#pragma config(Sensor, in2,    rightLift,      sensorPotentiometer)
#pragma config(Sensor, in3,    rotatorPot,     sensorPotentiometer)
#pragma config(Sensor, in4,    clawPot,        sensorPotentiometer)
#pragma config(Sensor, dgtl9,  leftDriveQuad,  sensorQuadEncoder)
#pragma config(Sensor, dgtl11, rightDriveQuad, sensorQuadEncoder)
#pragma config(Motor,  port2,           topLift,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           slingshot,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           intake,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           claw,          tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           frontRightDrive, tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           frontLeftDrive, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           backRightDrive, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           backLeftDrive, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          rotator,       tmotorVex393_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"




void pre_auton()
{

}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/



void runLeftDrive(int speed){
	motor[backLeftDrive] = speed;
	motor[frontLeftDrive] = speed;
}
void runRightDrive(int speed){
	motor[backRightDrive] = speed;
	motor[frontRightDrive] = speed;
}

void drive(int speed){
	runLeftDrive(speed);
	runRightDrive(speed);
}
void lift(int speed){
	motor[topLift]=speed;

}



typedef struct{
	float pGain;
	float iGain;
	float dGain;
	float iMin;
	float iMax;
	float iState;

	float position;
	float target;

	int lastRan;
	float prevError;
} PIDStruct;

PIDStruct rotatorPID;
int rotatorLowPos=605;
int rotatorHighPos=3850;


task rotatorPIDTask{
	while(1){
		if(vexRT[btn8R]){
			motor[rotator]=127;
			//wait1Msec(1100);
		}else if(vexRT[Btn8L]){
			motor[rotator]=-127;
		//wait1Msec(1100);
		}else{
			motor[rotator]=0;
		}
	}

}








//http://robotsforroboticists.com/pid-control/
int runPID(PIDStruct PIDData){
	//Update variables
	float timeInterval=PIDData.lastRan-nPgmTime;
	PIDData.lastRan=nPgmTime;
	float error=PIDData.target-PIDData.position;

	//Run proportional control
	int pTerm=PIDData.pGain*error;

	//Run integral control
	PIDData.iState+=error*timeInterval;
	int iTerm=PIDData.iGain*PIDData.iState;
	if(PIDData.iState>PIDData.iMax)PIDData.iState=PIDData.iMax;
	if(PIDData.iState<PIDData.iMin)PIDData.iState=PIDData.iMin;

	//Run derivative control
	int dTerm=PIDData.dGain*(error-PIDData.prevError)/(timeInterval+0.001);

	//Update variables for next run
	PIDData.prevError=error;

	return pTerm+iTerm+dTerm
}
PIDStruct liftPID
task liftControl{

	liftPID.pGain=0.35;
	liftPID.iGain=0;
	liftPID.dGain=0;
	liftPID.target=SensorValue[rightLift];
	while(1){

		if(vexRT[Btn6U]){
			//liftPID.target+=0.4;
			//if(liftPID.target>2300)liftPID.target=2300;
			liftPID.target=1900;
	}else if(vexRT[Btn5U]){
			liftPID.target=1450;
		}else if(vexRT[Btn6D]){
			liftPID.target=550;
			//liftPID.target-=0.4;
			//if(liftPID.target<640)liftPID.target=640;
		}
		lift(runPID(liftPID));

		liftPID.position=SensorValue[rightLift];

	}
}
int driveQuadAvg(){
	return SensorValue[rightDriveQuad]/2+SensorValue[leftDriveQuad]/2;
}
int driveQuadDiff(){
	return SensorValue[rightDriveQuad]/2-SensorValue[leftDriveQuad]/2;
}
void setDriveQuads(int n){
	SensorValue[rightDriveQuad]=n;
	SensorValue[leftDriveQuad]=n;
}
PIDStruct drivePID;
void pDrive(int distance){
		setDriveQuads(0);

		drivePID.pGain=0.2;
		drivePID.iGain=0;
		drivePID.dGain=0;
		drivePID.target=distance+driveQuadAvg();
		drivePID.position=driveQuadAvg();
		drive(runPID(drivePID));

		int counter=0;

		while(counter<300){
			if(abs(drivePID.target-drivePID.position)<80)counter++;
			else counter=0;
			drivePID.position=driveQuadAvg();
			drive(runPID(drivePID));
			wait1Msec(1);
		}
		drive(-sgn(distance)*10);
		wait1Msec(50);
		drive(0);

}
void pTurn(int distance){
		setDriveQuads(0);

		drivePID.pGain=0.4;
		drivePID.iGain=0;
		drivePID.dGain=0;
		drivePID.target=distance+driveQuadAvg();
		drivePID.position=driveQuadDiff();
		drive(runPID(drivePID));

		int counter=0;

		while(counter<300){
			if(abs(drivePID.target-drivePID.position)<80)counter++;
			else counter=0;
			drivePID.position=driveQuadDiff();
			int motorSpeed=runPID(drivePID);
			runRightDrive(motorSpeed);
			runLeftDrive(-motorSpeed);
			wait1Msec(1);
		}
		drive(-sgn(distance)*10);
		wait1Msec(50);
		drive(0);

}

void runClawPID(PIDStruct clawPID){

	clawPID.pGain=0.2;
	clawPID.iGain=0;
	clawPID.dGain=0;

	clawPID.position=SensorValue[clawPot];
	motor[claw]=-runPID(clawPID);
}

void runRotatorPID(PIDStruct rotatorPID){

	rotatorPID.pGain=0.1;
	rotatorPID.iGain=0;
	rotatorPID.dGain=0;

	rotatorPID.position=SensorValue[rotatorPot];
	motor[rotator]=-runPID(rotatorPID);
}

void nearAuton(int side){
	pDrive(-250);
	//motor[claw] = 20;
	startTask(liftControl);
	liftPID.target = 850;
	motor[slingshot]=127;
	wait1Msec(3500);
	motor[slingshot]=0;
	pTurn(82 * side);
	pDrive(-900);
	pTurn(60 * side);
	pDrive(2500);
	pTurn(50 * side)
	pDrive(7000)
}

void farAuton(int side){
//right is 1, left is -1
	motor[slingshot]=127;
	wait1Msec(2700);
	motor[slingshot]=0;
	pTurn(-800 * side);
	pDrive(5000);
	motor[claw] = -60;
	wait1Msec(600);
	pDrive(200);
	motor[claw] = 0;
	lift(70);
	motor[rotator] = 50;
	wait1Msec(1000);
	motor[rotator] = 0
	lift(0);
}

void farAuton2(int side){
	pDrive(-1100);
	motor[slingshot]=127;
	wait1Msec(2700);
	motor[slingshot]=0;
	wait1Msec(3000);
	pDrive(-1375);
	pTurn(1220 * side);
	pDrive(5000);
}

task autonomous()
{
	farAuton2(-1);
}


task usercontrol()
{
	startTask(liftControl);
	bool clawIdle=true;
	PIDStruct clawPID;

	rotatorPID.target=3850;
  while (true)
  {
		if(vexRT[Btn7U]){
			//this is made for testing auton
			//NEEDS TO BE COMMENTED OUT WHEN IN MATCHES
			nearAuton(1);

		}

		if( abs(vexRT[Ch1]) > 20 || abs(vexRT[Ch3]) > 20 ){
			runLeftDrive(vexRT[Ch3]);
			runRightDrive(vexRT[Ch2]);
		}else{
			drive(0);
		}


		if(vexRT[Btn8U]){
			motor[slingshot] = 127;
			liftPID.target=850;
		}else{
			motor[slingshot] = 0;
		}

		if(vexRT[Btn7U]){
			motor[intake] = 127;
		}else if(vexRT[Btn7D]){
			motor[intake] = -127;
		}else{
			motor[intake] = 0;
		}

		if(vexRT[Btn8UXmtr2]){
				liftPID.target=2300;
				wait1Msec(500);
				if(rotatorPID.target==rotatorLowPos)rotatorPID.target=rotatorHighPos;
				else if(rotatorPID.target==rotatorHighPos)rotatorPID.target=rotatorLowPos;



		}

  	if(vexRT[Btn7RXmtr2]){clawPID.target=850;clawIdle=false;}
  	else if(vexRT[Btn7LXmtr2]){ clawPID.target=50;clawIdle=false;}
  	if(vexRT[Btn8LXmtr2])rotatorPID.target=rotatorHighPos;
  	else if(vexRT[Btn8RXmtr2]) rotatorPID.target=rotatorLowPos;


  	if(vexRT[Btn7DXmtr2])clawIdle=true;

  	if(clawIdle)motor[claw]=0;
		else runClawPID(clawPID);
		runRotatorPID(rotatorPID);

  }
}
