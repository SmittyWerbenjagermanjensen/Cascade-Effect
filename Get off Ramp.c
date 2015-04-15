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
#pragma config(Motor,  mtr_S1_C2_1,     elevator,      tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     motorG,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     rightDrive,    tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     goalLifter,    tmotorTetrix, PIDControl, encoder)
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
	avgGyroX = avgGyroX/1000;
	avgGyroY = avgGyroY/1000;
}


float getTheta() {
	wait1Msec(5);
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
  //servo[score] = 0;
}


void goForward(int dist, int speed) { // PRECONDITION: dist > 0
	nMotorEncoder[leftDrive] = 0;
	nMotorEncoderTarget[leftDrive] = dist*80.27730236; // converts inches to ticks
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
	if (yaw > 0) {
		motor[rightDrive] = 30;
		while (getTheta() > 0) {}
		motor[rightDrive] = 0;
	}
	else if (yaw < 0) {
		motor[leftDrive] = 30;
		while (getTheta() < 0) {}
		motor[leftDrive] = 0;
	}
	yaw = 0;
}


void dropLifter() {
	nMotorEncoder[goalLifter] = 1440;
	nMotorEncoderTarget[goalLifter] = 0;
	motor[goalLifter] = -50;
	while(nMotorRunState[goalLifter] != runStateIdle) {}
	motor[goalLifter] = 0;
}


void pickUpGoal() {
	nMotorEncoder[goalLifter] = 0;
	nMotorEncoderTarget[goalLifter] = 100;
	motor[goalLifter] = 30;
	while(nMotorRunState[goalLifter] != runStateIdle) {}
	motor[goalLifter] = 0;
}


void putDownGoal() {
	nMotorEncoder[goalLifter] = 100;
	nMotorEncoderTarget[goalLifter] = 0;
	motor[goalLifter] = -30;
	while(nMotorRunState[goalLifter] != runStateIdle) {}
	motor[goalLifter] = 0;
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
	motor[elevator] = 80;
	wait1Msec(500);
	motor[elevator] = 0;
	servo[score] = 100;
	wait1Msec(1000);
	servo[score] = 0;
	motor[elevator] = -50;
	wait1Msec(200);
}


void scoreCenter() {
	motor[elevator] = 80;
	wait1Msec(300);
	motor[elevator] = 0;
	servo[autonScore] = 0;
	servo[score] = 100;
	wait1Msec(1000);
	servo[score] = 0;
	motor[elevator] = -50;
	wait1Msec(200);
}

int getOffRamp(int speed) {
	nMotorEncoder[rightDrive] = 0;
	nMotorEncoder[leftDrive] = 0;
	motor[rightDrive] = -speed;
	motor[leftDrive] = -speed;
	while(abs(getPitch()) < 10 || (nMotorEncoder[leftDrive] >= (22*80.27730236) && nMotorEncoder[rightDrive] >= (22*80.27730236))) {
	}
	while(abs(getPitch()) > 10 || (nMotorEncoder[leftDrive] >= (36*80.27730236) && nMotorEncoder[rightDrive] >= (36*80.27730236))) {
	}
	motor[rightDrive] = 0;
	motor[leftDrive] = 0;
	return nMotorEncoder[leftDrive];
}


task main()
{
  initializeRobot();

  //waitForStart(); // Wait for the beginning of autonomous phase.

  getOffRamp(50);

  while (true) {
  }
}
