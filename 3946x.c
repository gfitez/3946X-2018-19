#pragma config(Sensor, in1,    leftLift,       sensorPotentiometer)
#pragma config(Sensor, in2,    rightLift,      sensorPotentiometer)
#pragma config(Sensor, in3,    rotatorPot,     sensorPotentiometer)
#pragma config(Sensor, dgtl11, leftDriveQuad,  sensorQuadEncoder)
#pragma config(Motor,  port1,           claw,          tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           topLeftLift,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           topRightLift,  tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           bottomLeftLift, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           bottomRightLift, tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           rightDrive,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           leftDrive,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           slingshot,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           intake,        tmotorNone, openLoop)
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

task autonomous()
{

}

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
	motor[topRightLift]=speed;
	motor[topLeftLift]=speed;
	motor[bottomRightLift]=speed;
	motor[bottomLeftLift]=speed;
}
task clawControl{
	while(1){
		if(vexRT[Btn8R]){
			motor[rotator]=127;
			//wait1Msec(1100);
		}
		if(vexRT[Btn8L]){
			motor[rotator]=-127;
		//wait1Msec(1100);
		}
		motor[rotator]=0;
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
PIDStruct liftPID
task liftControl{

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
task usercontrol()
{
	startTask(clawControl);
	startTask(liftControl);

  while (true)
  {

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

		if(vexRT[Btn7U]){
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

  	if(vexRT[Btn7R]) motor[claw]=127;
  	else if(vexRT[Btn7L]) motor[claw]=-127;
		else motor[claw]=0;

  }
}
