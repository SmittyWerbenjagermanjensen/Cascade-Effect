#pragma config(Hubs,  S1, HTMotor,  none,     none,     none)
#pragma config(Motor,  mtr_S1_C1_1,     leftDrive,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     rightDrive,    tmotorTetrix, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.


void initializeRobot()
{


  return;
}


task main()
{
	motor[leftDrive] = 25;
	motor[rightDrive] = 25;
	wait1Msec(100);
	motor[leftDrive] = 50;
	motor[rightDrive] = 50;
	wait1Msec(100);
	motor[leftDrive] = 75;
	motor[rightDrive] = 75;
	wait1Msec(100);
	motor[leftDrive] = 100;
	motor[rightDrive] = 100;
	wait1Msec(100);
  //initializeRobot();

  //waitForStart();   // wait for start of tele-op phase

  //while (true)
  //{
		//getJoystickSettings(joystick);

		//if (abs(joystick.joy1_y1) > 8)
		//	motor[leftDrive] = joystick.joy1_y1;
		//if (abs(joystick.joy1_y2) > 8)
		//	motor[rightDrive] = joystick.joy1_y2;
  //}
}