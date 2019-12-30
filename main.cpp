#include "mbed.h"
#include "X8servo.hpp"
//#include "Timer.h"

InterruptIn hallInterrupt1(PE_9, PullUp);
InterruptIn hallInterrupt2(PE_11, PullUp);
DigitalOut led(LED3);

X8servo servo;
unsigned int ID = 0x141;
unsigned int ID1 = 0x141;
unsigned int ID2 = 0x142;

bool atZero = false;
float pi =  3.141592;
//Timer t;

/*
// only one ID can work
Thread servo1_thread;
Thread servo2_thread;

void servo1_worker(){
	while(true){
		servo.PositionControl2(ID1, 180.0, 1000.0);
		wait_ms(1000.0);
		servo.PositionControl2(ID2, 90.0, 1000.0);
		wait_ms(1000.0);
	}
}

void servo2_worker(){

	while(true){
		servo.PositionControl2(ID2, 180.0, 1000.0);
		wait_ms(1000.0);
		servo.PositionControl2(ID2, 90.0, 1000.0);
		wait_ms(1000.0);
	}
}
*/
void arrivedZero(){
	led = !led;		// just led out to see status
	atZero = true;  // 
}

void gotoZero(unsigned int id){

	float incTime = 0.0;
	float speed = 90.0;
	float delay = 0.0002;
	uint16_t offset;

	while (!atZero){
		speed = 90.0*sin(2.0*pi*incTime);  // randomly move with sinusoidal wave
		servo.SpeedControl(id, speed);	   // used speed control because smoother
		incTime = incTime + delay;
		wait(0.006+delay);
		}
	servo.MotorOff(id);					// this will kill the motor power
	wait(0.5);
	servo.PositionControl2(id,0.0, 60.0); // when motor start again it will go to the nearest zero
										   // and that is the point that we set at first in RMD config
	wait(1);
	atZero = false; 					   // set this flag to false to allow another id to use
}

int main()
{
	hallInterrupt1.rise(&arrivedZero);
	hallInterrupt1.fall(&arrivedZero);

	hallInterrupt2.rise(&arrivedZero);
	hallInterrupt2.fall(&arrivedZero);

	float inc = 0.0;
	float speed;
	float pos = 0.0;
	float delay = 0.001;

	gotoZero(ID1);
	wait(0.5);
	gotoZero(ID2);
	printf("done\n");

	//servo1_thread.start(servo1_worker);
	//servo2_thread.start(servo2_worker);

	
	//servo.PositionControl2(ID1,0.0, 360.0);  // time used 6~7ms
	//wait_ms(1000);
	/*
	servo.ReadMultiturn(ID1);
	servo.ReadEncoderData(ID1);
	printf("EncoderPosition ID%d: %d\n",(ID1-320), servo.EncoderPosition[ID1-320-1]);
	printf("EncoderOriginal ID%d: %d\n",(ID1-320), servo.EncoderOriginal[ID1-320-1]);
	printf("EncoderOffset ID%d: %d\n",(ID1-320), servo.EncoderOffset[ID1-320-1]);
	*/
	//servo.PositionControl2(ID2,0.0, 360.0);
	//servo.SpeedControl(ID, 1500.0);

	
	//servo.PositionControl2(ID1,0.0, 360.0);
	//servo.PositionControl2(ID2,0.0, 360.0);
	//wait(2.0);
	/*
	servo.ReadEncoderData(ID1);
	printf("EncoderPosition ID%d: %d\n",(ID1-320), servo.EncoderPosition[ID1-320-1]);
	printf("EncoderOriginal ID%d: %d\n",(ID1-320), servo.EncoderOriginal[ID1-320-1]);
	printf("EncoderOffset ID%d: %d\n",(ID1-320), servo.EncoderOffset[ID1-320-1]);

	offset = servo.EncoderPosition[ID1-320-1];
	//servo.ReadEncoderData(ID2);
	//printf("EncoderPosition ID%d: %d\n",(ID2-320), servo.EncoderPosition[ID2-320-1]);
	//printf("EncoderOriginal ID%d: %d\n",(ID2-320), servo.EncoderOriginal[ID2-320-1]);
	//printf("EncoderOffset ID%d: %d\n",(ID2-320), servo.EncoderOffset[ID2-320-1]);
	
	servo.WriteEncoderOffset(ID1, offset);
	wait(1.0);

	servo.ReadEncoderData(ID1);
	printf("EncoderPosition ID%d: %d\n",(ID1-320), servo.EncoderPosition[ID1-320-1]);
	printf("EncoderOriginal ID%d: %d\n",(ID1-320), servo.EncoderOriginal[ID1-320-1]);
	printf("EncoderOffset ID%d: %d\n",(ID1-320), servo.EncoderOffset[ID1-320-1]);
	
	wait(1.0);
	servo.PositionControl2(ID1,90.0, 360.0);
	wait(1.0);
	servo.PositionControl2(ID1,45.0, 360.0);
	wait(1.0);
	servo.PositionControl2(ID1,120.0, 360.0);
	wait(1.0);
	servo.PositionControl2(ID1,0.0, 360.0);
	wait(1.0);
	
	servo.ReadEncoderData(ID1);
	printf("EncoderPosition ID%d: %d\n",(ID1-320), servo.EncoderPosition[ID1-320-1]);
	printf("EncoderOriginal ID%d: %d\n",(ID1-320), servo.EncoderOriginal[ID1-320-1]);
	printf("EncoderOffset ID%d: %d\n",(ID1-320), servo.EncoderOffset[ID1-320-1]);
	*/

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
		/*
		// This is to test two servos ID 1 and 2 
		servo.PositionControl2(ID1,45.0, 2500.0);
		//wait_ms(2000);
		//servo.ReadMultiturn(ID1);
		//wait_ms(1000);
		servo.PositionControl2(ID2,90.0, 2500.0);
		//wait_ms(2000);
		//servo.ReadMultiturn(ID2);
		//wait_ms(1000);
		servo.PositionControl2(ID1, 200.0, 2500.0);
		//wait_ms(2000);
		//servo.ReadMultiturn(ID1);
		//wait_ms(1000);
		servo.PositionControl2(ID2,180.0, 2500.0);
		//wait_ms(2000);
		//servo.ReadMultiturn(ID2);
		//wait_ms(1000);
		*/
		
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