#pragma config(Hubs,  S1, HTMotor,  HTServo,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     sensorSonar,    sensorSONAR)
#pragma config(Sensor, S3,     sensorSignal,   sensorLightActive)
#pragma config(Motor,  motorA,          motorLiftLeft, tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          motorLiftRight, tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          motorSweep,    tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     motorRight,    tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     motorLeft,     tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Servo,  srvo_S1_C2_1,    servoDoor,            tServoStandard)
#pragma config(Servo,  srvo_S1_C2_2,    servoAuto,            tServoStandard)
#pragma config(Servo,  srvo_S1_C2_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//


#define DRIVE_FORWARD_ENCODER_COUNT 5000
#define DUMP_POSITION 125
#define DOOR_CLOSE_POSITION 0
#define DOOR_GROUND_LEVEL_OPEN 82
#define LIFT_ENCODER_COUNT 2700
#define NXT_MOTOR_MAX 100
#define NXT_MOTOR_STOP 0
#define DC_MOTOR_MAX 10
#define DC_MOTOR_STOP 0
#define TICKS_PER_INCH 204
#define BOX_SPACING TICKS_PER_INCH*10.5
#define DISTANCE_VALUE_CLOSE 50
#define DISTANCE_VALUE_FAR 36
#define HIGH_POSITION 245
#define MEDIUM_POSITION 200
#define BANG_POSITION 75
#define LOW_POSITION 0

int lift_encoder = LIFT_ENCODER_COUNT;
int drivepower = DC_MOTOR_MAX;
//int distanceArray;

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                           Autonomous Mode Code Template
//
// This file contains a template for simplified creation of an autonomous program for an Tetrix robot
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

void drive(int drive_encoder_left, int drive_encoder_right);
void lift(int liftpower);
void door(int position);
void scan_beacon_locations(int beacon_spacing);
void easy_autonomous();
void ramp();
void no_sonar_autonomous();

void initializeRobot()
{
    // Place code here to initialize servos to starting positions.
    // Sensors are automatically configured and setup by ROBOTC. They may need a brief time to stabilize.
    nMotorEncoder[motorLeft] = 0;
    nMotorEncoder[motorRight] = 0;
    servoTarget[servoDoor] = DOOR_GROUND_LEVEL_OPEN;  //Set the servo door into the ground level open position
		servoTarget[servoAuto] = LOW_POSITION;
    return;
}

void no_sonar_autonomous()
{
	servo[servoAuto] = HIGH_POSITION;
	motor[motorRight] = drivepower;	//Robot goes forward up until first box
	motor[motorLeft] = drivepower;
	wait1Msec(3700);
	motor[motorRight] = DC_MOTOR_STOP;
	motor[motorLeft] = DC_MOTOR_STOP;
	for(int i=0; i < 2; i++)    //Bang arm twice
  {
    servoTarget[servoAuto] = BANG_POSITION;   //Should be in box
    wait1Msec (1000);
    servoTarget[servoAuto] = MEDIUM_POSITION;   //Should give arm enough room to bang block without losing it
    wait1Msec (1000);
  }
   servoTarget[servoAuto] = HIGH_POSITION;   //Arm goes back to intial position
   motor[motorRight] = -20;
   motor[motorLeft] = -20;
   wait1Msec(1600);	//Robot goes backward for 1.5 seconds
   motor[motorRight] = -7;
   motor[motorLeft] = 15;
   wait1Msec(2100);	//Robot turns left for 2 seconds
   motor[motorRight] = -20;
   motor[motorLeft] = -20;
   wait1Msec(4200);	//Robot reverses for 2.2 seconds
   motor[motorLeft] = 15;
   motor[motorRight] = -25;
   servoTarget[servoAuto] = LOW_POSITION; //Arm is down
   wait1Msec(1600);	//Robot turns left for 2 seconds
   //motor[motorLeft] = -20;
   //motor[motorRight] = -20;
   //wait1Msec();	//Robot reverses for 2.05 seconds
   //motor[motorLeft] = 25;
   //motor[motorRight] = -23;
   //wait1Msec(875);	//Robot point tunrns for .975 seconds
	 //wait1Msec(3000);
	 motor[motorLeft] = -100;
	 motor[motorRight] = -100;
	 wait1Msec(2000);	//Robt reverses for 2 seconds
	 motor[motorLeft] = DC_MOTOR_STOP;
	 motor[motorRight] = DC_MOTOR_STOP;	//Auto ends
}




void ramp()
{
		while(SensorValue(sensorSonar) > DISTANCE_VALUE_FAR) //Check this threshold. Might need to be higher
   	{
        motor[motorLeft] = drivepower;  //Robot keep driving until it sees way too much
        motor[motorRight] = drivepower; //At that point, it will have passed the box see saw
    }
    motor[motorLeft] = DC_MOTOR_STOP;
    motor[motorRight] = DC_MOTOR_STOP;
    motor[motorLeft] = drivepower-7; //Once robot passes box see saw, it time to turn
    motor[motorRight] = drivepower; //May want to use sync_motors and encoders. Waiting *might* work?
    //Now robot needs to turn left and then back up at full power onto the ramp
    //Encoders ._________.
    //After ramp() is done, call it into easy_autonomous. Check code on Wednesday!
    //Could use 4 element array. Might take more time, but will be more advanced...but also more of a pain.
    wait1Msec(2000);
		motor[motorLeft] = drivepower;
    motor[motorRight] = drivepower;
    wait1Msec(2000);
    motor[motorLeft] = DC_MOTOR_STOP;
    motor[motorRight] = DC_MOTOR_STOP;
}


void easy_autonomous()
{
    servoTarget[servoAuto] = HIGH_POSITION; //Should this just be in initialize phase?
    while(SensorValue(sensorSonar) > DISTANCE_VALUE_CLOSE) //The threshold may need to be higher to account for issues
    {
        motor[motorLeft] = drivepower;  //Robot Drive forward until it spots the ir sensor...
        motor[motorRight] = drivepower;
    }
    motor[motorLeft] = DC_MOTOR_STOP;
    motor[motorRight] = DC_MOTOR_STOP;
    //wait1Msec (500);    //give robot some time to stop before autoarm phase
    for(int i=0; i < 2; i++)    //Bang arm twice
    {
        servoTarget[servoAuto] = LOW_POSITION;   //Should be in box
        wait1Msec (1000);
        servoTarget[servoAuto] = MEDIUM_POSITION;   //Should give arm enough room to bang block without losing it
        wait1Msec (1000);
    }
    servoTarget[servoAuto] = HIGH_POSITION;   //Arm goes back to intial position
    }



void scan_beacon_locations(int beacon_spacing)
{
    int distanceArray[4];
    for (int i = 0;  i < 4; i++)  // i is beacon index
    {
        // for each beacon location...
        distanceArray[i] = SensorValue[sensorSonar];    //Sonar sensor gets the 4 array values

        // TODO: MOVE ROBOT box_spacing ENCODER TICKS
        drive(BOX_SPACING, BOX_SPACING);  //Robot moves 10.5 inches to get under each box
    }
}

void drive(int drive_encoder_left, int drive_encoder_right)
{
    nMotorEncoderTarget[motorLeft] += drive_encoder_left;   //Motor encoder target equal motor encoder target plus
    nMotorEncoderTarget[motorRight] += drive_encoder_right;
    motor[motorLeft] = -drivepower;
    motor[motorRight] = -drivepower;
    while((nMotorRunState[motorLeft] != runStateIdle) && (nMotorRunState[motorRight] != runStateIdle))
    {

    }
    motor[motorLeft] = DC_MOTOR_STOP;
    motor[motorRight] = DC_MOTOR_STOP;
}

void lift(int liftpower)
{
    nMotorEncoderTarget[motorLiftLeft] = lift_encoder;
    motor[motorLiftLeft] = liftpower;
    while(nMotorRunState[motorLiftLeft] != runStateIdle)
    {

    }
    motor[motorLiftLeft] = NXT_MOTOR_STOP;
}

void door(int position) //Function will allow user to close door at 0* on the servo.
{
    servoTarget[servoDoor] = position;
    if(position == DOOR_CLOSE_POSITION)
    {
        SensorType[sensorSignal] = sensorLightInactive;
    }
    else if(position == DOOR_GROUND_LEVEL_OPEN || position == DUMP_POSITION)
    {
        SensorType[sensorSignal] = sensorLightActive;
    }
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

    no_sonar_autonomous();
		while(true)
		{
			//No need for this except for format.
		}


}
