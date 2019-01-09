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
			if(SensorValue[rightLift]<2000-200)liftPID.target=2000;
			else liftPID.target+=0.4;
	}else if(vexRT[Btn5U]){
			if(SensorValue[rightLift]<1500-200)liftPID.target=1500;
			else liftPID.target+=0.4;


		}else if(vexRT[Btn6D] && liftPID.target>650){
			liftPID.target-=0.4;
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
task rotatorTask(){
		while(1)runRotatorPID(rotatorPID);
}
bool clawIdle=false;
PIDStruct clawPID;
task clawTask(){
	while(1){
		if(clawIdle)motor[claw]=0;
			else runClawPID(clawPID);
	}
}
int autonTime=0;
void nearAuton(int side){
	clearTimer(T1);
	startTask(rotatorTask);
	startTask(liftControl);

	liftPID.target = 850;
	pDrive(-1100);//Drive back to hit flag
	motor[slingshot]=127;
	//turns claw towards ground
	rotatorPID.target = rotatorLowPos;
	pDrive(760);//drive back
	pTurn(-75*side);//angle to shoot at mid flag
	wait1Msec(800);
	motor[slingshot]=0;
	liftPID.target = 500;//put lift on ground for claw to grab cap
	startTask(clawTask);
	clawPID.target=800;
	pTurn(410 * side);//turn to face cap
	pDrive(650);
	clawPID.target=50;//grap cap

	rotatorPID.target=rotatorHighPos
	liftPID.target=850;//lift up
	pDrive(300);
	clawPID.target=800;

	pTurn(-380*side);//turn to face platform
	drive(127);
	wait1msec(1900);
	drive(-50);
	wait1Msec(50);
	drive(0);


	autonTime=time1[T1];

}

void farAuton(int side){
//red is 1, blue is -1
	clearTimer(T1);
	rotatorPID.target=SensorValue[rotatorPot];
	startTask(rotatorTask);
	startTask(liftControl);
	startTask(clawTask);

	liftPID.target = 850;
	motor[slingshot]=127;
	pDrive(-300);//Drive back to hit flag
	wait1Msec(2000);
	motor[slingshot]=0;
	//turns claw towards ground
	pTurn(208*side);//angle to shoot at mid flag

	rotatorPID.target = rotatorLowPos;
	liftPID.target = 575;//put lift on ground for claw to grab cap

	pDrive(1000);
	clawPID.target=850;
	pDrive(500);
	clawPID.target=50;
	rotatorPID.target=rotatorHighPos

	pTurn(475*side);
	drive(127);
	wait1msec(750);
	drive(-50);
	wait1Msec(50);
	drive(0);
	pDrive(-300);
	pTurn(300*side);
	liftPID.target=850;//lift up

	drive(127);
	wait1msec(1800);
	drive(-50);
	clawPID.target=850;
	wait1Msec(50);
	drive(0);


	autonTime=time1[T1];
}


task autonomous()
{
	nearAuton(1);
}



task usercontrol()
{
	startTask(liftControl);


	startTask(rotatorTask);
	startTask(clawTask);
	rotatorPID.target=3850;
  while (true)
  {
		if(vexRT[Btn5d]){
			//this is made for testing auton
			//NEEDS TO BE COMMENTED OUT WHEN IN MATCHES
			//nearAuton2(1);

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



  }
}
