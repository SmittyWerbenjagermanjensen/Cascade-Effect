#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     yawDetector,    sensorI2CHiTechnicGyro)
#pragma config(Sensor, S3,     pitchDetector,  sensorI2CHiTechnicGyro)
#pragma config(Sensor, S4,     irDetector,     sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     leftDrive,     tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     mainIntake,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     elevator,      tmotorTetrix, PIDControl)
#pragma config(Motor,  mtr_S1_C2_2,     motorG,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     rightDrive,    tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     goalLifter,    tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Servo,  srvo_S1_C4_1,    score,                tServoStandard)
#pragma config(Servo,  srvo_S1_C4_2,    autonScore,           tServoStandard)
#pragma config(Servo,  srvo_S1_C4_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "drivers/hitechnic-gyro.h";
#include "drivers/hitechnic-accelerometer.h";
#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.


const int retracted = 1200;
const int lifted = 350;
const int bottom = 0;

int goalPos = 1;

float yaw = 0;
float avgGyroX = 0;
float pitch = 0;
float avgGyroY = 0;


void calibrateSensors() {

PlaySound(soundLowBuzz);
	 	for (int i = 0; i < 1000; i ++) {
			avgGyroX += SensorValue[yawDetector];
			avgGyroY += SensorValue[pitchDetector];
			wait1Msec(1);
		}
	ClearTimer(T1);
	avgGyroX = avgGyroX/1000;
	avgGyroY = avgGyroY/1000;
}


float getTheta() {
	yaw += ((float)SensorValue[yawDetector]-avgGyroX) * time1[T1] / 1000.0;
	ClearTimer(T1);
	return (yaw);
}

float getPitch() {
	pitch = pitch + ((float)SensorValue[pitchDetector]-avgGyroY) * time1[T1] / 1000.0;
	ClearTimer(T1);
	return (pitch);
}


void initializeRobot()
{
  calibrateSensors();
  //servo[score] = 234;
  servo[autonScore] = 100;
  servo[score] = 212;
  yaw = 0;
}

void gyroTurn(int targetDegrees, int speed) {
	if (getTheta() < targetDegrees) {
		motor[leftDrive] = speed;
	}
	else {
		motor[leftDrive] = -speed;
	}
	while (getTheta() < targetDegrees-1 || getTheta() > targetDegrees) {
	}
	motor[leftDrive] = 0;
	motor[rightDrive] = 0;
	if (speed >= 10) {
		gyroTurn(targetDegrees, speed/2);
	}
}

void goForward(int dist, int speed) { // PRECONDITION: dist > 0
	nMotorEncoder[leftDrive] = 0;
	nMotorEncoder[rightDrive] = 0;
	nMotorEncoderTarget[leftDrive] = dist*80.27730236; // converts inches to ticks
  nMotorEncoderTarget[rightDrive] = dist*80.27730236; // converts inches to ticks
	motor[leftDrive] = speed;
	motor[rightDrive] = speed;
	while(nMotorRunState[leftDrive] != runStateIdle && nMotorRunState[rightDrive] != runStateIdle) {
	}
	motor[leftDrive] = 0;
	motor[rightDrive] = 0;
}


void goBackward(int dist, int speed) { // PRECONDITION: dist > 0
	nMotorEncoder[leftDrive] = 0;
	nMotorEncoder[rightDrive] = 0;
	nMotorEncoderTarget[leftDrive] = dist*80.27730236; // converts inches to ticks
	nMotorEncoderTarget[rightDrive] = dist*80.27730236; // converts inches to ticks
	motor[leftDrive] = -speed;
	motor[rightDrive] = -speed;
	while(nMotorRunState[leftDrive] != runStateIdle && nMotorRunState[rightDrive] != runStateIdle) {
		getTheta();
	}
	motor[leftDrive] = 0;
	motor[rightDrive] = 0;
}


void turnRight(int angle, int speed) { // PRECONDITION: angle > 0
	yaw = 0;
	while (getTheta() < angle) {
		motor[leftDrive] = speed;
		motor[rightDrive] = -speed;
		wait1Msec(5);
	}
	motor[leftDrive] = 0;
	motor[rightDrive] = 0;

	/*if (speed >= 30)
		turnRight(abs(getTheta()-angle), speed/2); // adjust at the end
		*/
}

void gyroTurnRight(int angle, int speed) { // PRECONDITION: angle > 0
	yaw = 0;
	while (getTheta() < angle) {
		motor[leftDrive] = speed;
		motor[rightDrive] = -speed;
		wait1Msec(5);
	}
	motor[leftDrive] = 0;
	motor[rightDrive] = 0;
	gyroTurn(angle, 10);
}

void turnLeft(int angle, int speed) { // PRECONDITION: angle > 0
	yaw = 0;
	while (-getTheta() < angle) {
		motor[leftDrive] = -speed;
		motor[rightDrive] = speed;
		wait1Msec(5);
	}
	motor[leftDrive] = 0;
	motor[rightDrive] = 0;
	/*
	if (speed >= 30)
		turnRight(abs(getTheta()-angle), speed/2); // adjust at the end
		*/
}


void pivotForwardOnRight(int ticks, int speed) { // PRECONDITION: angle > 0
	nMotorEncoder[leftDrive] = 0;
	nMotorEncoderTarget[leftDrive] = ticks;  // converts inches to ticks
	motor[leftDrive] = speed;
	while (nMotorRunState[leftDrive] != runStateIdle){
	}
	motor[leftDrive] = 0;
}

	void gyroPivotForwardOnRight(int angle, int speed) { // PRECONDITION: angle > 0
	yaw = 0;
	while (getTheta() < angle) {
		motor[leftDrive] = speed;
		wait1Msec(5);
	}
	motor[leftDrive] = 0;
	gyroTurn(angle, 10);
}

  /*
	nMotorEncoder[leftDrive] = 0;
	nMotorEncoderTarget[leftDrive] = ((90*(PI/180))*14)*80.27730236);  // converts inches to ticks
	motor[leftDrive] = speed;
	while (nMotorRunState[leftDrive] != runStateIdle){
	}
	motor[leftDrive] = 0;
	*/

	/*yaw = 0;
	while (getTheta() < angle) {
	 	motor[leftDrive] = speed;
	}
	motor[leftDrive] = 0;
	/*
	if (abs(getTheta() - angle) > 5) {
		pivotBackOnRight(abs(getTheta()-angle), -(speed/2)); // adjust at the end
	}

}*/


void pivotBackOnLeft(int ticks, int speed) { // PRECONDITION: angle > 0
	nMotorEncoder[rightDrive] = 0;
	nMotorEncoderTarget[rightDrive] = -ticks;  // converts inches to ticks
	motor[rightDrive] = -speed;
	while (nMotorRunState[rightDrive] != runStateIdle){
	}
	motor[leftDrive] = 0;
/*
	yaw = 0;
	while (getTheta() < angle) {
		motor[rightDrive] = -speed;
		wait1Msec(5);
	}
	motor[leftDrive] = 0;

	if (speed >= 30)
		pivotBackOnLeft(-abs(getTheta()-angle), -speed/2); // adjust at the end
		*/
}


void realign() { // turns the robot forward
	nxtDisplayCenteredTextLine(5, "%d", getTheta());
	if (getTheta() > 5) {
		motor[leftDrive] = -15;
		while (getTheta() > 5) {}
		motor[rightDrive] = 0;
	}
	else if (getTheta() < 5) {
		motor[rightDrive] = -15;
		while (getTheta() < 5) {}
		motor[rightDrive] = 0;
	}
	yaw = 0;
	PlaySound(soundBlip);
}


void deployLifter() {
	nMotorEncoder[goalLifter] = bottom;
	while(nMotorEncoder[goalLifter] > -retracted) {
		motor[goalLifter] = -50;
	}
	motor[goalLifter] = 0;
	nMotorEncoder[goalLifter] = bottom;
}


void undeployLifter() {
	while(nMotorEncoder[goalLifter] < retracted) {
		motor[goalLifter] = 50;
	}
	motor[goalLifter] = 0;
}


void pickUpGoal() {
	while(nMotorEncoder[goalLifter] < lifted) {
		motor[goalLifter] = 30;
	}
	motor[goalLifter] = 0;
}


void putDownGoal() {
	while(nMotorEncoder[goalLifter] > bottom) {
		motor[goalLifter] = -30;
	}
	motor[goalLifter] = 0;
}


void swerveBackwardWhileDeployingLifter() { // 52 is the distance
	nMotorEncoder[goalLifter] = bottom;
	nMotorEncoder[leftDrive] = 0;
	nMotorEncoder[rightDrive] = 0;
	nMotorEncoderTarget[leftDrive] = 52*80.27730236; // converts inches to ticks
	nMotorEncoderTarget[rightDrive] = 52*80.27730236; // converts inches to ticks
	motor[leftDrive] = -50;
	motor[rightDrive] = -50;
	while(nMotorEncoder[goalLifter] > -retracted && nMotorRunState[leftDrive] != runStateIdle && nMotorRunState[rightDrive] != runStateIdle) {
		motor[goalLifter] = -50;
	}
	motor[goalLifter] = 0;
	while(nMotorRunState[leftDrive] != runStateIdle && nMotorRunState[rightDrive] != runStateIdle) {
		getTheta();
	}
	motor[rightDrive] = 0;
	motor[leftDrive] = 0;
	nMotorEncoder[goalLifter] = bottom;
}

void straightenForCascade() {
	switch(goalPos) {
		case 1:
			//gyroTurn(-65, 10);
		break;

		case 2:
			gyroTurn(-74, 10);
		break;

		case 3:
			gyroTurn(-82, 10);
		break;
	}
}

void findIR() {
	for (int i = 0; i < 2; i ++) {
		if (goalPos == 2) {
			goBackward(4, 35);
		}
  	if (SensorValue[irDetector] == 5) {
  		//PlaySound(soundUpwardTones);
  		return;
	  }
	  goalPos++;
	  wait1Msec(100);
	  PlaySound(soundBeepBeep);
  	goBackward(18, 35);
  	pivotBackOnLeft(1250, 65);
  	goBackward(4, 35);
  	wait1Msec(100);
  }

  PlaySound(soundException); // if it doesn't find it, cry out in agony
}


void scoreMedium() {
//	motor[elevator] = 80;
	wait1Msec(500);
//	motor[elevator] = 0;
	servo[score] = 100;
	wait1Msec(1000);
	servo[score] = 0;
//	motor[elevator] = -50;
	wait1Msec(200);
}


void scoreTall() {
//	motor[elevator] = 80;
	wait1Msec(500);
//	motor[elevator] = 0;
	servo[score] = 100;
	wait1Msec(1000);
	servo[score] = 0;
//	motor[elevator] = -50;
	wait1Msec(200);
}


void scoreCenter() {
	motor[elevator] = 100;
	wait1Msec(3000);
	motor[elevator] = 0;
	for (int i = 0; i < 3; i++) {
		wait1Msec(400);
		motor[elevator] = 100;
		wait1Msec(250);
		motor[elevator] = 0;
	}
		//servo[autonScore] = 127;
	wait1Msec(100);
	goBackward(7, 25);
	servo[autonScore] = 250;
	wait1Msec(500);
	servo[score] = 127;
	wait1Msec(1500);
	servo[autonScore] = 100;
	goForward(7, 25);
	//servo[score] = \158;
	motor[elevator] = -10;
	wait1Msec(750);
	motor[elevator] = 0;
}


int getOffRamp(int speed) {
	nMotorEncoder[rightDrive] = 0;
	nMotorEncoder[leftDrive] = 0;
	motor[rightDrive] = -speed;
	motor[leftDrive] = -speed;
	while(abs(getPitch()) < 10 || (nMotorEncoder[leftDrive] >= (22*80.27730236) && nMotorEncoder[rightDrive] >= (22*80.27730236))) {
	}
	PlaySound(soundDownwardTones);
	while(abs(getPitch()) > 0 || (nMotorEncoder[leftDrive] >= (36*80.27730236) && nMotorEncoder[rightDrive] >= (36*80.27730236))) {
	}
	PlaySound(soundFastUpwardTones);
	motor[rightDrive] = 0;
	motor[leftDrive] = 0;
	return nMotorEncoder[leftDrive];
}


void getOffRampAndGetGoal() {
	nMotorEncoder[leftDrive] = 0;
	nMotorEncoder[rightDrive] = 0;
	motor[leftDrive] = -50;
	motor[rightDrive] = -50;
	while (abs(nMotorEncoder[leftDrive]) < 4174) {}
	PlaySound(soundBeepBeep);
	nMotorEncoder[goalLifter] = 0;
	motor[goalLifter] = -50;
	while(abs(nMotorEncoder[leftDrive]) < 8028 && abs(nMotorEncoder[rightDrive]) < 8028) {
		if (nMotorEncoder[goalLifter] <= -retracted) {
			motor[goalLifter] = 0;
		}
		motor[rightDrive] = -50;
		motor[leftDrive] = -50;
	}
	motor[leftDrive] = 0;
	motor[rightDrive] = 0;
	nMotorEncoder[goalLifter] = bottom;
}


void stopMotors() {
	motor[rightDrive] = 0;
	motor[leftDrive] = 0;
}


void runAutonomous() { // runs second half of SMITTY-AP (from the parking zone)
	goForward(20, 35);
	pivotForwardOnRight(2475, 40);
	//gyroPivotForwardOnRight(90, 50);
	goBackward(15, 40);
	findIR();
	if (goalPos == 2) {
		goBackward(7, 30);
	}
	else if (goalPos == 3) {
		goBackward(7, 30);
	}
	else {
		goBackward(11, 30);
	}
	//gyroTurnRight(90, 50);
	turnRight(80, 50);
	wait1Msec(200);
	motor[leftDrive] = -35;
	motor[rightDrive] = -35;
	wait1Msec(1500);
	motor[leftDrive] = 0;
	motor[rightDrive] = 0;
	wait1Msec(100);
	goForward(9, 25);
	scoreCenter();
	//PlaySound(soundLowBuzz);
	wait1Msec(100);
	turnLeft(70, 50);
	goForward(10, 40);
	pivotForwardOnRight(2000, 65);
	straightenForCascade();

	for (int i = 0; i < 1; i++) {
		goBackward(40, 100);
		goForward(36, 100);
	}

	//deployLifter();


	/*
	wait1Msec(2000);
	turnLeft(90, 50);
	goForward(30, 50);
	turnLeft(90, 50);
	while (true) {
		goForward(24, 100);
		PlaySound(soundUpwardTones);
		goBackward(24, 100);
		PlaySound(soundDownwardTones);
	}
	*/
}


task main()
{
  initializeRobot();

  waitForStart(); // Wait for the beginning of autonomous phase.
  PlaySound(soundBeepBeep);

  runAutonomous();

  while (true) {
  }
}
