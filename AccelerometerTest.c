#pragma config(Sensor, S1,     HTAC,           sensorI2CHiTechnicAccel)
#pragma config(Sensor, S2,     SENSOR_GYRO_X,  sensorI2CHiTechnicGyro)
#pragma config(Motor,  motorA,          Right,         tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          Left,          tmotorNXT, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
 * $Id: hitechnic-accelerometer-test1.c 133 2013-03-10 15:15:38Z xander $
 */

/**
 * hitechnic-accelerometer.h provides an API for the HiTechnic Acceleration Sensor.  This program
 * demonstrates how to use that API.
 *
 * Changelog:
 * - 0.1: Initial release
 * - 0.2: Make use of new API calls
 * - 0.3: Better comments
 * - 0.4: Fixed display line (thanks Dave)
 * - 0.5: Removed single axis functions, they're no longer in the driver\n
 *        Removed common.h from includes
 *
 * Credits:
 * - Big thanks to HiTechnic for providing me with the hardware necessary to write and test this.
 *
 * License: You may use this code as you wish, provided you give credit where it's due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 3.59 AND HIGHER.

 * Xander Soldaat (xander_at_botbench.com)
 * 20 February 2011
 * version 0.5
 */

#include "drivers/hitechnic-accelerometer.h"

task main () {
  int _x_axis = 0;
  int _y_axis = 0;
  int _z_axis = 0;
  float avgZAxis = 0;
  float zAccel = 0;
  float zVelocity = 0;

  string _tmp;

  nxtDisplayCenteredTextLine(0, "HiTechnic");
  nxtDisplayCenteredBigTextLine(1, "Accel");
  nxtDisplayCenteredTextLine(3, "Test 1");
  nxtDisplayCenteredTextLine(5, "Connect sensor");
  nxtDisplayCenteredTextLine(6, "to S1");
  wait1Msec(2000);

  PlaySound(soundBeepBeep);
  while(bSoundActive) EndTimeSlice();

	PlaySound(soundLowBuzz);
	 for (int i = 0; i < 1000; i ++) {
	  HTACreadAllAxes(HTAC, _x_axis, _y_axis, _z_axis);
		avgZAxis += _z_axis;
		wait1Msec(1);
  }
  avgZAxis = avgZAxis/1000;

  while (true) {
    eraseDisplay();
		HTACreadAllAxes(HTAC, _x_axis, _y_axis, _z_axis);
    nxtDisplayTextLine(0,"HTAC Test 1");

    // We can't provide more than 2 parameters to nxtDisplayTextLine(),
    // so we'll do in two steps using StringFormat()
		wait1Msec(1);
    zAccel = _z_axis-avgZAxis;
    zVelocity = zVelocity + ((float)zAccel) * time1[T1] / 1000.0;
    ClearTimer(T1);

    nxtDisplayTextLine(2, "   X    Y    Z");
    StringFormat(_tmp, "%4d %4d", _x_axis, _y_axis);
    nxtDisplayTextLine(3, "%s %4f", _tmp, zVelocity);
 		nxtDisplayTextLine(4, "%s %4f", _tmp, zAccel);
 		nxtDisplayTextLine(5, "%s %4f", _tmp, _z_axis);
    wait1Msec(100);
  }
}

/*
 * $Id: hitechnic-accelerometer-test1.c 133 2013-03-10 15:15:38Z xander $
 */
