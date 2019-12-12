#include "mbed.h"
#include "X8servo.hpp"
//#include "Timer.h"

X8servo servo;
unsigned int ID = 0x141;
unsigned int ID1 = 0x141;
unsigned int ID2 = 0x142;
//Timer t;

int main()
{
	float inc = 0.0;
	float speed;
	float pos = 0.0;
	float pi =  3.141592;
	float delay = 0.001;

	
	//servo.PositionControl2(ID,180.0, 360.0);  // time used 6~7ms

	while (1){
		
		/*
		// This block is to test sine-wave driven
		pos = 180.0*cos(2.0*pi*inc);
		servo.PositionControl2(ID, pos, 1000.0);
		inc = inc + delay;
		wait(0.015+delay);
		*/
		
		/*
		// Test write multi turn and sleep
		servo.PositionControl2(ID,0.0, 2500.0);
		wait_ms(1000);
		servo.PositionControl2(ID,90.0, 2500.0);
		wait_ms(1000);
		servo.PositionControl2(ID,-180.0, 2500.0);
		wait_ms(1000);
		servo.PositionControl2(ID,-720.0, 2500.0);
		wait_ms(1000);
		*/

		// This is to test two servos ID 1 and 2 
		servo.PositionControl2(ID1,600.0, 2500.0);
		wait_ms(1000);
		servo.PositionControl2(ID2,90.0, 2500.0);
		wait_ms(1000);
		servo.PositionControl2(ID1, 310.0, 2500.0);
		wait_ms(1000);
		servo.PositionControl2(ID2,-180.0, 2500.0);
		wait_ms(1000);

		
		/*
		// This block is to test Stop and Run command
		servo.SpeedControl(ID, 180.0);
		wait(2.0);
		servo.MotorStop(ID);
		wait(2.0);
		servo.MotorRun(ID);
		*/

		/*
		// This is to test ReadMultiTurn after run
		servo.PositionControl2(ID, 270.0, 180.0);  // time used 5ms
		wait(3.0);
		servo.ReadMultiturn(ID);		// time used 2ms
		wait(1.0);
		servo.PositionControl2(ID, 420.0, 180.0);
		wait(3.0);
		servo.ReadMultiturn(ID);
		wait(1.0);
		*/

		/*
		// Control mode 3 doesn't work, don't know why...?
		servo.PositionControl3(ID, 90.0, 0x01);
		wait(2);
		servo.ReadMotorStatusError(ID);
		wait(1);
		servo.PositionControl3(ID, 60.0, 0x00);
		wait(2);
		servo.ReadMotorStatusError(ID);
		wait(1);
		*/
		
		/*
		// This is to test position control mode 4
		servo.PositionControl4(ID, 90.0, 90.0, 0x01);
		wait(2);
		servo.PositionControl4(ID, 45.0, 90.0, 0x00);
		wait(2);
		*/

	}



}