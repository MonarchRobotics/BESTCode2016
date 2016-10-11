#pragma config(Motor,  port2,           rightMotor,    tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port3,           cornGrabber,   tmotorServoStandard, openLoop)
#pragma config(Motor,  port4,           boomMotor,     tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port6,           leftMotor,     tmotorServoContinuousRotation, openLoop, reversed)
#pragma config(Motor,  port7,           cornDoor,      tmotorServoStandard, openLoop)
#pragma config(Motor,  port8,           seedControl,   tmotorServoStandard, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*----------------------------------------------------------------------------------------------------*\
|*                           - Dual Joystick Control with Arm - 1 Remote -                            *|
|*                                      ROBOTC on VEX 2.0 Cortex                                      *|
|*                                                                                                    *|
|*  This program uses both the Left and the Right joysticks to run the robot using "tank control".    *|
|*  The Group 6 buttons on the top-right of the VEXnet Joystick are used to raise and lower an arm.   *|
|*                                                                                                    *|
|*                                        ROBOT CONFIGURATION                                         *|
|*    NOTES:                                                                                          *|
|*    1)  Ch1 is the X axis and Ch2 is the Y axis for the RIGHT joystick.                             *|
|*    2)  Ch3 is the Y axis and Ch4 is the X axis for the LEFT joystick.                              *|
|*                                                                                                    *|
|*    MOTORS & SENSORS:                                                                               *|
|*    [I/O Port]              [Name]              [Type]              [Description]                   *|
|*    Motor - Port 2          rightMotor          VEX Motor           Right motor                     *|
|*    Motor - Port 6          leftMotor           VEX Motor           Left motor                      *|
|*    Motor - Port 4          boomMotor           VEX Motor           Boom Extension Control          *|
|*    Motor - Port 3          cornGrabber         Servo               Corn Grabber                    *|
|*    Motor - Port 8          seedControl         Servo               Seed Door Control               *|
|*    Motor - Port 7          cornDoor            Servo               Corn Bay Door Control           *|
So seed needs to stay closed until opened,so we can drive along to planting
Then, when opened, needs to stay opened until closed so we can drive along planting
Then, when closed, needs to stay closed until opened so we can drive back and get more seeds
make a seedState variable to store needed position, have button presses change it.
\*----------------------------------------------------------------------------------------------------*/

//+++++++++++++++++++++++++++++++++++++++++++++| MAIN |+++++++++++++++++++++++++++++++++++++++++++++++

//this var manages position of seedHopper, so don't have to hold it open while driving.
int seedState = -25;
int cornState = -120;
int openLimit = 120;
int closeLimit = -114;
int gripState = -107;

task main()
{

	while(1 == 1)
	{
		//Driving Motor Control
		motor[leftMotor] = vexRT[Ch3] / 2;//Left Joystick Up/Down
		motor[rightMotor] = vexRT[Ch2] / 2;//Right Joystick Up/Down

		//Corn Door  Control
		if(vexRT[Btn8U] == 1 && vexRT[Btn8R] == 1)//Right Side Up/Right Buttons
		{
			cornState *= -1;
			delay(200);
		}
		motor[cornDoor] = cornState;

		//Seed door control
		//Seed control
		if(vexRT[Btn8D] == 1 && vexRT[Btn8L] == 1)//Right Side Down/Left Buttons
		{
			seedState *= -1;
			delay(200);
			//added this to prevent rapid oscillation between states, buffer zone
		}
		motor[seedControl] = 95 + seedState;
		motor[seedControl] = -60 + seedState;

		//Corn Grabber Control
		if(vexRT[Btn7U] == 1 && vexRT[Btn7L] == 1)//Left Side Up/Left Buttons
		{
			//motor[cornGrabber] = 120; //open
			if(gripState < openLimit){
				gripState += 5;
				delay(25);
			}

		}
		else if(vexRT[Btn7D] == 1 && vexRT[Btn7R] == 1)//Left Side Down/Right Buttons
		{
			//motor[cornGrabber] = -120; //close
			if(gripState > closeLimit){
				gripState -= 5;
				delay(25);
			}
		}
		motor[cornGrabber] = gripState;


		/*
		*	Store state, while holding a button increment by 1, stop when released or reach limit of Grabber
		*	find out an open limit and a closed limit. Start with -120 to 0, change from there
		*/

		//Boom Control
		if(vexRT[Btn6U] == 1 || vexRT[Btn5U] == 1)//Upper Shoulder Buttons
		{
			motor[boomMotor] = 160;
		}
		else if(vexRT[Btn6D] == 1 || vexRT[Btn5D] == 1)//Lower Shoulder Buttons
		{
			motor[boomMotor] = -160;
		}
		else
		{
			motor[boomMotor] = 0;
		}
	}
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
