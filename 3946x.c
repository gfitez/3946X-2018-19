#pragma config(Sensor, in1,    leftLift,       sensorPotentiometer)
#pragma config(Sensor, in2,    rightLift,      sensorPotentiometer)
#pragma config(Sensor, in3,    rotatorPot,     sensorPotentiometer)
#pragma config(Sensor, in4,    clawPot,        sensorPotentiometer)
#pragma config(Sensor, dgtl9,  leftDriveQuad,  sensorQuadEncoder)
#pragma config(Sensor, dgtl11, rightDriveQuad, sensorQuadEncoder)
#pragma config(Motor,  port2,           topLift,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           bottomLift,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           claw,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           rotator,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           rightDrive,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           leftDrive,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           slingshot,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           intake,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          rotator,       tmotorNone, openLoop)
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
	motor[leftDrive] = speed;
}
void runRightDrive(int speed){
	motor[rightDrive] = speed;
}
void drive(int speed){
	runLeftDrive(speed);
	runRightDrive(speed);
}
void lift(int speed){
	motor[topLift]=speed;
	motor[bottomLift]=speed;

}
task rotatorPID{
	while(1){
		if(vexRT[Btn8R]){
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

task liftControl{
	PIDStruct liftPID
	liftPID.pGain=0.35;
	liftPID.iGain=0;
	liftPID.dGain=0;
	liftPID.target=SensorValue[rightLift];
	while(1){

		if(vexRT[Btn6U]){
			liftPID.target=SensorValue[rightLift];
			lift(90);
		}
		if(vexRT[Btn6D]){
			liftPID.target=SensorValue[rightLift];
			lift(-40);
		}
		liftPID.position=SensorValue[rightLift];
		lift(runPID(liftPID));
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

	rotatorPID.pGain=0.4;
	rotatorPID.iGain=0;
	rotatorPID.dGain=0;

	rotatorPID.position=SensorValue[rotatorPot];
	motor[rotator]=-runPID(rotatorPID);
}

void nearAuton(int side){
	pDrive(-2900);
	motor[slingshot]=127;
	wait1Msec(2700);
	motor[slingshot]=0;
	pTurn(200 * side);
	pDrive(-1700);
	//pTurn(130 * side);
	pDrive(7300);
	pTurn(1020 * side)
	pDrive(3600)
}

void farAuton(int side){
	pTurn(-1300 * side);
	pDrive(2500);
	pTurn(1300 * side);
	pDrive(3000);
	motor[claw] = 50;
	pDrive(600);
	motor[claw] = -50;


}
task autonomous()
{
	nearAuton(-1);
}
task usercontrol()
{
	startTask(liftControl);
	bool clawClosed=true;
	PIDStruct clawPID;
	PIDStruct rotatorPID;
	rotatorPID.target=4050;
  while (true)
  {
		if(vexRT[Btn5D]){
			//farAuton(-1);
			nearAuton(-1);
			//pTurn(1000);
		}
		if( abs(vexRT[Ch1]) > 10 || abs(vexRT[Ch3]) > 10 ){
			runLeftDrive(vexRT[Ch3]);
			runRightDrive(vexRT[Ch2]);
		}else{
			drive(0);
		}


		if(vexRT[Btn8U]){
			motor[slingshot] = 127;
		}else{
			motor[slingshot] = 0;
		}

		if(vexRT[Btn5U]){
			motor[intake] = 127;
		}else{
			motor[intake] = 0;
		}

		/**if(vexRT[Btn6U]){
			lift(127);
		}else if(vexRT[Btn6D]){
			lift(-60);
		}else{
			lift(0);
  	}**/

  	if(vexRT[Btn7R])clawPID.target=1100;
  	else if(vexRT[Btn7L]) clawPID.target=0;
  	if(vexRT[Btn8L])rotatorPID.target=4050;
  	else if(vexRT[Btn8R]) rotatorPID.target=3200;

		runClawPID(clawPID);
		runRotatorPID(rotatorPID);

  }
}
