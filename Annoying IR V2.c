#pragma config(Sensor, S1, IR1, sensorHiTechnicIRSeeker1200)

task main()
{
  int IR1value;
  while(true)
  {
    IR1value = SensorValue[IR1];
    switch (IR1value) {
    	case 0:
    		PlayTone(100, 1);
    	break;

    	case 1:
    		PlayTone(1000, 1);
    	break;

    	case 2:
    		PlayTone(1250, 1);
    	break;

    	case 3:
    		PlayTone(1500, 1);
    	break;

    	case 4:
    		PlayTone(1750, 1);
    	break;

	    case 5:
	    	PlayTone(2000, 1);
	    break;

	    case 6:
    		PlayTone(2250, 1);
    	break;

    	case 7:
    		PlayTone(2500, 1);
    	break;

    	case 8:
    		PlayTone(2750, 1);
    	break;

    	case 9:
    		PlayTone(3000, 1);
    	break;
    }
    nxtDisplayCenteredBigTextLine(5,"%d",IR1value);
  }
}
