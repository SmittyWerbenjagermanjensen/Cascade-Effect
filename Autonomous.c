#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S2,     yawDetector,    sensorI2CHiTechnicGyro)
#pragma config(Sensor, S3,     accelerometer,  sensorI2CHiTechnicAccel)
#pragma config(Sensor, S4,     irDetection,    sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  mtr_S1_C1_1,     rightDrive,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     leftDrive,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     mainIntake,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     unspecified1,  tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     unspecified2,  tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     unspecified2,  tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C4_1,    rightIntake,          tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C4_2,    leftIntake,           tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C4_3,    deployIntake,         tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C4_4,    score,                tServoStandard)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "drivers/hitechnic-gyro.h";
#include "drivers/hitechnic-accelerometer.h";

float thetaX = 0;
float avgGyroX = 0;
bool finished = false;
int xAxis = 0;
int yAxis = 0;
int zAxis = 0;
float avgXAxis = 0;
float avgYAxis = 0;
float avgZAxis = 0;

void calibrateSensors() {
	while (avgGyroX < 570 || avgGyroX > 630) {
		PlaySound(soundLowBuzz);
	 	for (int i = 0; i < 1000; i ++) {
			avgGyroX += SensorValue[yawDetector];
			wait1Msec(1);
		}
	}
	avgGyroX = avgGyroX/1000;
	PlaySound(soundLowBuzz);
	 for (int i = 0; i < 1000; i ++) {
	 	HTACreadAllAxes(accelerometer, xAxis, yAxis, zAxis);
		avgXAxis += xAxis;
		avgYAxis += yAxis;
	  avgZAxis += zAxis;
		wait1Msec(1);
  }
  avgZAxis = avgZAxis/1000;
}

int getTheta() {
	thetaX = thetaX + ((float)SensorValue[yawDetector]-avgGyroX) * time1[T1] / 1000.0;
	ClearTimer(T1);
	return (int)(thetaX);
}

void testForOffRamp() {
	bool offRamp = false;
	while (!offRamp) {
	}
}

//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                           Autonomous Mode Code Template
//
// This file contains a template for simplified creation of an autonomous program for an TETRIX robot
// competition.
//
// You need to customize two functions with code unique to your specific robot.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                    initializeRobot
//
// Prior to the start of autonomous mode, you may want to perform some initialization on your robot.
// Things that might be performed during initialization include:
//   1. Move motors and servos to a preset position.
//   2. Some sensor types take a short while to reach stable values during which time it is best that
//      robot is not moving. For example, gyro sensor needs a few seconds to obtain the background
//      "bias" value.
//
// In many cases, you may not have to add any code to this function and it will remain "empty".
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

void initializeRobot()
{
  // Place code here to sinitialize servos to starting positions.
  // Sensors are automatically configured and setup by ROBOTC. They may need a brief time to stabilize.

  return;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                         Main Task
//
// The following is the main code for the autonomous robot operation. Customize as appropriate for
// your specific robot.
//
// The types of things you might do during the autonomous phase (for the 2008-9 FTC competition)
// are:
//
//   1. Have the robot follow a line on the game field until it reaches one of the puck storage
//      areas.
//   2. Load pucks into the robot from the storage bin.
//   3. Stop the robot and wait for autonomous phase to end.
//
// This simple template does nothing except play a periodic tone every few seconds.
//
// At the end of the autonomous period, the FMS will autonmatically abort (stop) execution of the program.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

task main()
{
  initializeRobot();

  waitForStart(); // Wait for the beginning of autonomous phase.

  ///////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////
  ////                                                   ////
  ////    Add your robot specific autonomous code here.  ////
  ////                                                   ////
  ///////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////

  while (true)
  {
  	calibrateSensors();
  	motor[rightDrive] = -25;
  	motor[leftDrive] = -25;
  }
}
