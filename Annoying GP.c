#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     yawDetector,    sensorI2CHiTechnicGyro)
#pragma config(Sensor, S3,     accelerometer,  sensorI2CHiTechnicAccel)
#pragma config(Sensor, S4,     irDetection,    sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     rightDrive,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     leftDrive,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     mainIntake,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     unspecified1,  tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     unspecified2,  tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     unspecified2,  tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C4_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*const tSensors GyroSensor = (tSensors) S1;
#define offset 603
long Gyro_value;

task main() {
	while(true) {
	Gyro_value = SensorValue(GyroSensor)-offset;
	nxtDisplayCenteredTextLine(3, "%f", Gyro_value);
	}
}
*/
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!

#include "drivers\hitechnic-gyro.h";
float thetaX = 0;
float avgGyroX = 0;
string yaw = "YAW:";

task main(){
		//nxtDisplayCenteredTextLine(2, "%s", "Starting:")
	 // thetaX = (int)(rand()*180-90);
	while (avgGyroX < 570 || avgGyroX > 630) {
		PlaySound(soundLowBuzz);
	 	for (int i = 0; i < 1000; i ++) {
			avgGyroX += SensorValue[SENSOR_GYRO_X];
			wait1Msec(1);
  	}
  	avgGyroX = avgGyroX/1000;
  }
 	PlayTone(1500, 5);

 	thetaX = 0;
	while(abs((thetaX)) < 45) {
		motor[Right] = 25;
		motor[Left] = -25;
		wait1Msec(1);
		thetaX = thetaX + ((float)SensorValue[SENSOR_GYRO_X]-avgGyroX) * time1[T1] / 1000.0;
		ClearTimer(T1);


		//int refX = (int)(thetaX+180)%360 - 180;
	}
	eraseDisplay();
	nxtDisplayTextLine(2, "%s", yaw);
	nxtDisplayCenteredTextLine(5, "%d", thetaX);
	motor[Right] = 0;
	motor[Left] = 0;
}
