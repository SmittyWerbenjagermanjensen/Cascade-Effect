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
#pragma config(Motor,  mtr_S1_C2_1,     elevator,      tmotorTetrix, openLoop)
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


const int retracted = 1175;
const int lifted = 325;
const int partialLifted = 75;
const int bottom = 0;

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
	yaw = yaw + ((float)SensorValue[yawDetector]-avgGyroX) * time1[T1] / 1000.0;
	ClearTimer(T1);
	return (yaw);
}

float getPitch() {
	wait1Msec(5);
	pitch = pitch + ((float)SensorValue[pitchDetector]-avgGyroY) * time1[T1] / 1000.0;
	ClearTimer(T1);
	return (pitch);
}


void initializeRobot()
{
  calibrateSensors();
  servo[score] = 162;
  servo[autonScore] = 100;
  yaw = 0;
}


void goForward(int dist, int speed) { // PRECONDITION: dist > 0
	nMotorEncoder[leftDrive] = 0;
	nMotorEncoder[rightDrive] = 0;
	nMotorEncoderTarget[leftDrive] = dist*80.27730236; // converts inches to ticks
  nMotorEncoderTarget[rightDrive] = dist*80.27730236; // converts inches to ticks
	motor[leftDrive] = speed;
	motor[rightDrive] = speed;
	while(nMotorRunState[leftDrive] != runStateIdle && nMotorRunState[rightDrive] != runStateIdle) {
		getTheta();
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
}


void pivotBackOnRight(int angle, int speed) { // PRECONDITION: angle > 0 < speed
	yaw = 0;
	while (-getTheta() < angle) {
		motor[leftDrive] = -speed;
		wait1Msec(5);
	}
	motor[leftDrive] = 0;
}


void pivotForwardOnRight(int angle, int speed) { // PRECONDITION: angle > 0
	yaw = 0;
	while (getTheta() < angle) {
		motor[leftDrive] = speed;
		wait1Msec(5);
	}
	motor[leftDrive] = 0;
}


void pivotBackOnLeft(int angle, int speed) { // PRECONDITION: angle > 0
	yaw = 0;
	while (getTheta() < angle) {
		motor[rightDrive] = -speed;
		wait1Msec(5);
	}
	motor[rightDrive] = 0;
}


void pivotForwardOnLeft(int angle, int speed) { // PRECONDITION: angle > 0
	yaw = 0;
	while (-getTheta() < angle) {
		motor[rightDrive] = speed;
		wait1Msec(5);
	}
	motor[rightDrive] = 0;
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
	ClearTimer(T2);
	while(nMotorEncoder[goalLifter] > -retracted && time1[T2] < 1500) {
		motor[goalLifter] = -50;
	}
	motor[goalLifter] = 0;
	nMotorEncoder[goalLifter] = bottom;
}

void undeployLifter() {
	ClearTimer(T2);
	while(nMotorEncoder[goalLifter] < retracted && time1[T2] < 1500) {
		motor[goalLifter] = 50;
	}
	motor[goalLifter] = 0;
}


void pickUpGoal() {
	ClearTimer(T2);
	while(nMotorEncoder[goalLifter] < lifted && time1[T2] < 1000) {
		motor[goalLifter] = 30;
	}
	motor[goalLifter] = 0;
	PlaySound(soundBeepBeep);
}


void putDownGoal() {
	ClearTimer(T2);
	while(nMotorEncoder[goalLifter] > bottom && time1[T2] < 1000 ) {
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


void findIR() {
	for (int i = 0; i < 3; i ++) {
  	if (SensorValue[irDetector] == 5)
	  	return;
  	goBackward(12, 50);
  	pivotBackOnLeft(45, 50);
  	goBackward(8, 50);
  }

  PlaySound(soundException); // if it doesn't find it, cry out in agony
}


void scoreMedium() {
	motor[elevator] = 100;
	wait1Msec(1250);
	motor[elevator] = 0;
	wait1Msec(100);
	servo[autonScore] = 250;
	wait1Msec(500);
	servo[score] = 127;
	wait1Msec(1500);
	servo[autonScore] = 100;
	motor[elevator] = -10;
	wait1Msec(200);
	motor[elevator] = 0;
}


void scoreTall() {
	motor[elevator] = 80;
	wait1Msec(100);
	motor[elevator] = 0;
	servo[autonScore] = 127;
	servo[score] = 243;
	wait1Msec(1000);
	servo[score] = 158;
	motor[elevator] = -15;
	wait1Msec(100);
	motor[elevator] = 0;
}


void scoreCenter() {
	//motor[elevator] = 80;
	wait1Msec(300);
	//motor[elevator] = 0;
	servo[autonScore] = 0;
	servo[score] = 100;
	wait1Msec(1000);
	servo[score] = 0;
	//motor[elevator] = -50;
	wait1Msec(200);
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

void getOffRampAndGetGoal() {
	/*nMotorEncoder[leftDrive] = 0;
	nMotorEncoder[rightDrive] = 0;
	motor[leftDrive] = -30;
	motor[rightDrive] = -30;
	while (abs(nMotorEncoder[leftDrive]) < 4600) {}
	PlaySound(soundBeepBeep);
	motor[leftDrive] = 0;
	while (abs(nMotorEncoder[rightDrive]) < 4600) {}
	motor[rightDrive] = 0;
	//gyroTurn(5, 10);*/
	nMotorEncoder[goalLifter] = 0;
	//motor[goalLifter] = -50;
	nMotorEncoderTarget[leftDrive] = 3800;
	nMotorEncoderTarget[rightDrive] = 3800;
	motor[leftDrive] = -32;
	motor[rightDrive] = -32;
	motor[goalLifter] = -35;
	bool isRetracted = false;
	ClearTimer(T2);
	while((nMotorRunState[leftDrive] != runStateIdle && nMotorRunState[rightDrive] != runStateIdle) || time1[T2] < 500) {
		if (nMotorEncoder[goalLifter] <= -retracted) {
			motor[goalLifter] = 0;
			nMotorEncoder[goalLifter] = bottom;
			isRetracted = true;
		}

	 if ((abs(nMotorEncoder[leftDrive]) > 3600 || abs(nMotorEncoder[rightDrive]) > 3600) && isRetracted) {
			if (nMotorEncoder[goalLifter] <= partialLifted) {
				motor[goalLifter] = 35;
			}
			else {
				motor[goalLifter] = 0;
			}
		}
	}
	motor[leftDrive] = 0;
	motor[rightDrive] = 0;
	motor[goalLifter] = 0;
	nMotorEncoder[goalLifter] = bottom;
}


void align() {
	turnLeft(75, 30);
	motor[leftDrive] = 30;
	motor[rightDrive] = 30;
	wait1Msec(2000);
	yaw = 0;
	wait1Msec(200);
	goBackward(2, 30);
	turnRight(83, 30);
	wait1Msec(200);
	gyroTurn(81, 10);
}


void runAutonomous() { // scores over 200 pts for autonomous
	/*goBackwardCoast(58, 50);
	//realign();
	swerveBackwardWhileDeployingLifter();s
	*/
	goBackward(63, 50);
	align();
	getOffRampAndGetGoal();
	pickUpGoal();
	scoreMedium();
	wait1Msec(100);
	pivotForwardOnRight(25, 50);
	goForward(100, 50);
	turnRight(140, 50);
	//putDownGoal();
	while (true) {
	}
	goForward(4, 50);
	pivotForwardOnLeft(35, 50);
	turnRight(140, 30);
	goBackward(3, 30);
	goBackward(10, 50);
	pickUpGoal();
	scoreTall();
	goForward(100, 50);
	turnRight(152, 40);
	putDownGoal();
	goForward(12, 50);
}


task main()
{
  initializeRobot();

  //waitForStart(); // Wait for the beginning of autonomous phase.

  runAutonomous();

  while (true) {
  }
}
