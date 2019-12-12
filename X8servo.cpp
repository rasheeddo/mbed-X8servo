#include "mbed.h"
#include "X8servo.hpp"
#include "rtos.h"

CAN can1(PB_8,PB_9);  // RD then TD  (PD_0,PD_1)
Serial pc(USBTX, USBRX,115200);

X8servo::X8servo(){

	can1.frequency(1000000);

}


void X8servo::Int16ToByteData(unsigned int Data, unsigned char StoreByte[2]){

	StoreByte[0] = (Data & 0xFF00) >> 8;
	StoreByte[1] = (Data & 0x00FF);

}

void X8servo::Int32ToByteData(unsigned int Data, unsigned char StoreByte[4]){

	StoreByte[0] = (Data & 0xFF000000) >> 24;
	StoreByte[1] = (Data & 0x00FF0000) >> 16;
	StoreByte[2] = (Data & 0x0000FF00) >> 8;
	StoreByte[3] = (Data & 0x000000FF);

}

uint16_t X8servo::ByteDataToInt16(unsigned char ByteData[2]){

	uint16_t IntData;

	IntData = (ByteData[0] & 0xFF) | ((ByteData[1] & 0xFF) << 8);

	return IntData;
} 

int64_t X8servo::ByteDataToInt64(unsigned char ByteData[8]){

	int64_t IntData;

	IntData = (ByteData[1] & 0xFF) | ((ByteData[2] & 0xFF) << 8) | ((ByteData[3] & 0xFF) << 16) | ((ByteData[4] & 0xFF) << 24) | ((ByteData[5] & 0xFF) <<  32) | ((ByteData[6] & 0xFF) << 40) | ((ByteData[7] & 0xFF) << 48) ;

	return IntData;
}

long X8servo::map(long x, long in_min, long in_max, long out_min, long out_max) 
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void X8servo::CANErrorCheck(){
	// If there is an error of tx or rx caused by overflow ( still don't know where there is overflow...)
	// the servo will move for a while then stop forever even we keep sending command
	if (can1.rderror() || can1.tderror()){
		pc.printf("rderror: %d\n", can1.rderror());
		pc.printf("tderror: %d\n", can1.tderror());
		//can1.reset();
	}
}

void X8servo::canWrite(unsigned int _ID, char data[8]){
	
	int i;
	// create a CANMessage object
	CANMessage sendMsg = CANMessage(_ID, data, 8, CANData, CANStandard);
	
	// keep sending it until there is nothing to send
	do{
		i = can1.write(sendMsg);
	}
	while(!i);
	//wait_us(100);
}

void X8servo::readReplyFlush(unsigned int _ID){

	pc.printf("readReplyFlush\n");
	CANMessage replyMsg;
	while (can1.read(replyMsg)){
		/*
		pc.printf("ID: %d\n", (replyMsg.id-320));
		pc.printf("data[0]: %X\n", replyMsg.data[0]);
		pc.printf("data[1]: %X\n", replyMsg.data[1]);
		pc.printf("data[2]: %X\n", replyMsg.data[2]);
		pc.printf("data[3]: %X\n", replyMsg.data[3]);
		pc.printf("data[4]: %X\n", replyMsg.data[4]);
		pc.printf("data[5]: %X\n", replyMsg.data[5]);
		pc.printf("data[6]: %X\n", replyMsg.data[6]);
		pc.printf("data[7]: %X\n", replyMsg.data[7]);
		*/
	}

}

bool X8servo::readReply(unsigned int _ID, unsigned char output[8]){

	pc.printf("readReply\n");
	CANMessage replyMsg;
	bool readOK = false;
	while (can1.read(replyMsg)){
		/*
		pc.printf("ID: %d\n", (replyMsg.id-320));
		pc.printf("data[0]: %X\n", replyMsg.data[0]);
		pc.printf("data[1]: %X\n", replyMsg.data[1]);
		pc.printf("data[2]: %X\n", replyMsg.data[2]);
		pc.printf("data[3]: %X\n", replyMsg.data[3]);
		pc.printf("data[4]: %X\n", replyMsg.data[4]);
		pc.printf("data[5]: %X\n", replyMsg.data[5]);
		pc.printf("data[6]: %X\n", replyMsg.data[6]);
		pc.printf("data[7]: %X\n", replyMsg.data[7]);
		*/
		output[0] = replyMsg.data[0];
		output[1] = replyMsg.data[1];
		output[2] = replyMsg.data[2];
		output[3] = replyMsg.data[3];
		output[4] = replyMsg.data[4];
		output[5] = replyMsg.data[5];
		output[6] = replyMsg.data[6];
		output[7] = replyMsg.data[7];

		readOK = true;
		
	}

	return readOK;

}

int64_t X8servo::ReadMultiturn(unsigned int _ID){
	uint64_t startTime = rtos::Kernel::get_ms_count();
	pc.printf("ReadMultiturn\n");
	unsigned char reply[8];
	int64_t output;
	char CommandByte[8] = {0x92, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	canWrite(_ID, CommandByte);
	
	if(readReply(_ID, reply)){

		output = ByteDataToInt64(reply);
		output = (output/(100*gearRatio));   // 0.01 deg/LSB

	}
	else{
		output = NULL;
		pc.printf("unable to read output\n");
	}

	pc.printf("output: %lld\n", output);

	CANErrorCheck();
	uint64_t stopTime = rtos::Kernel::get_ms_count();
	uint64_t period_ms = stopTime - startTime;
	pc.printf("Time Used %f ms\n", (float)period_ms);

	return output;
}

void X8servo::ReadMotorStatusError(unsigned int _ID){
	
	pc.printf("ReadMultiturn\n");
	unsigned char reply[8];
	unsigned char voltByte[2];
	unsigned char tempByte;
	float output_volt;
	uint8_t output_temp;

	char CommandByte[8] = {0x9A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	canWrite(_ID, CommandByte);

	if (readReply(_ID, reply)){

		tempByte = reply[1];
		voltByte[0] = reply[3];
		voltByte[1] = reply[4];
		output_volt = ByteDataToInt16(voltByte)*0.1;
		output_temp = (int)tempByte;
		pc.printf("voltage: %f\n", output_volt);
		pc.printf("temperature: %d\n", output_temp);

	}

	CANErrorCheck();

	if ((int)reply[7] > 0){
		switch ((int)reply[7]){
			case 1 :
				pc.printf("Low voltage protection error\n");
				break;
			case 8 :
				pc.printf("Over temperature protection error\n");
				break;
			case 9 :
				pc.printf("Low voltage and Over temp error\n");
				break;

		}
	}

}


void X8servo::MotorOff(unsigned int _ID){
	pc.printf("MotorOff\n");
	char CommandByte[8] = {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	canWrite(_ID, CommandByte);
	readReplyFlush(_ID);
	CANErrorCheck();
}

void X8servo::MotorStop(unsigned int _ID){
	pc.printf("MotorStop\n");
	char CommandByte[8] = {0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	canWrite(_ID, CommandByte);
	readReplyFlush(_ID);
	CANErrorCheck();
}

void X8servo::MotorRun(unsigned int _ID){
	pc.printf("MotorRun\n");
	char CommandByte[8] = {0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	canWrite(_ID, CommandByte);
	readReplyFlush(_ID);
	CANErrorCheck();
}

void X8servo::SpeedControl(unsigned int _ID, float DPS){

	pc.printf("SpeedControl\n");
	pc.printf("DPS %f\n", DPS);
	float SpeedLSB = DPS*100*gearRatio;
	unsigned char SpeedByte[4];
	char CommandSpeedByte[8];
	Int32ToByteData((int)SpeedLSB, SpeedByte);
	CommandSpeedByte[0] = 0xA2;
	CommandSpeedByte[1] = 0x00;
	CommandSpeedByte[2] = 0x00;
	CommandSpeedByte[3] = 0x00;
	CommandSpeedByte[4] = SpeedByte[3];
	CommandSpeedByte[5] = SpeedByte[2];
	CommandSpeedByte[6] = SpeedByte[1];
	CommandSpeedByte[7] = SpeedByte[0];
	canWrite(_ID, CommandSpeedByte);
	// the servo will reply everytime we send command to it
	// readReplyFlush seem like to eliminate the tderror and rderror after sending command
	readReplyFlush(_ID);
	CANErrorCheck();

}

void X8servo::PositionControl1(unsigned int _ID, float Deg){

	float DegLSB = Deg*100*gearRatio;
	unsigned char PositionByte[4];
	char CommandPosition1Byte[8];
	Int32ToByteData((int)DegLSB, PositionByte);
	CommandPosition1Byte[0] = 0xA3;
	CommandPosition1Byte[1] = 0x00;
	CommandPosition1Byte[2] = 0x00;
	CommandPosition1Byte[3] = 0x00;
	CommandPosition1Byte[4] = PositionByte[3];
	CommandPosition1Byte[5] = PositionByte[2];
	CommandPosition1Byte[6] = PositionByte[1];
	CommandPosition1Byte[7] = PositionByte[0];
	canWrite(_ID, CommandPosition1Byte);
	// the servo will reply everytime we send command to it
	// readReplyFlush seem like to eliminate the tderror and rderror after sending command
	readReplyFlush(_ID);
	CANErrorCheck();

}

void X8servo::PositionControl2(unsigned int _ID, float Deg, float DPS){
	//uint64_t startTime = rtos::Kernel::get_ms_count();
	pc.printf("PositionControl2\n");
	pc.printf("Target Output Deg %f and DPS %f\n", Deg, DPS);
	float DegLSB = Deg*100*gearRatio;
	float SpeedLSB = DPS*gearRatio;

	unsigned char PositionByte[4];
	unsigned char SpeedByte[2];

	Int32ToByteData((int)DegLSB, PositionByte);
	Int16ToByteData((int)SpeedLSB, SpeedByte);	// This two conversion functions takes 5ms most of the time

	char CommandPosition2Byte[8];

	CommandPosition2Byte[0] = 0xA4;
	CommandPosition2Byte[1] = 0x00;
	CommandPosition2Byte[2] = SpeedByte[1];
	CommandPosition2Byte[3] = SpeedByte[0];
	CommandPosition2Byte[4] = PositionByte[3];
	CommandPosition2Byte[5] = PositionByte[2];
	CommandPosition2Byte[6] = PositionByte[1];
	CommandPosition2Byte[7] = PositionByte[0];
	// until here use 5ms

	canWrite(_ID, CommandPosition2Byte);		// write and read function is super fast totally just 2ms
	// until here still 5ms
	// the servo will reply everytime we send command to it
	// readReplyFlush seem like to eliminate the tderror and rderror after sending command
	readReplyFlush(_ID);  // if there is printf it will take 15ms if no printf it's 7ms

	CANErrorCheck(); // 

	//uint64_t stopTime = rtos::Kernel::get_ms_count();
	//uint64_t period_ms = stopTime - startTime;
	//pc.printf("Time Used %f ms\n", (float)period_ms);

}

void X8servo::PositionControl3(unsigned int _ID, float Deg, unsigned char Direction){

	// This mode 3 still doesn't work I don't know what happend...
	pc.printf("PositionControl3\n");
	pc.printf("Target Output Deg %f and Turn %X\n", Deg, Direction);
	float DegLSB = Deg*100*gearRatio;
	unsigned char PositionByte[2];

	Int16ToByteData((int)DegLSB, PositionByte);

	char CommandPosition3Byte[8];

	CommandPosition3Byte[0] = 0xA5;
	CommandPosition3Byte[1] = Direction;
	CommandPosition3Byte[2] = 0x00;
	CommandPosition3Byte[3] = 0x00;
	CommandPosition3Byte[4] = PositionByte[1];
	CommandPosition3Byte[5] = PositionByte[0];
	CommandPosition3Byte[6] = 0x00;
	CommandPosition3Byte[7] = 0x00;
	canWrite(_ID, CommandPosition3Byte);
	// the servo will reply everytime we send command to it
	// readReplyFlush seem like to eliminate the tderror and rderror after sending command
	readReplyFlush(_ID);
	CANErrorCheck();

}

void X8servo::PositionControl4(unsigned int _ID, float Deg, float DPS, unsigned char Direction){

	pc.printf("PositionControl4\n");
	pc.printf("Target Output Deg %f and DPS %f and Direction %d\n", Deg, DPS, Direction);
	float DegLSB = Deg*100*gearRatio;
	float SpeedLSB = DPS*gearRatio;;

	unsigned char PositionByte[2];
	unsigned char SpeedByte[2];

	Int16ToByteData((int)DegLSB, PositionByte);
	Int16ToByteData((int)SpeedLSB, SpeedByte);

	char CommandPosition4Byte[8];

	CommandPosition4Byte[0] = 0xA6;
	CommandPosition4Byte[1] = Direction;
	CommandPosition4Byte[2] = SpeedByte[1];
	CommandPosition4Byte[3] = SpeedByte[0];
	CommandPosition4Byte[4] = PositionByte[1];
	CommandPosition4Byte[5] = PositionByte[0];
	CommandPosition4Byte[6] = 0x00;
	CommandPosition4Byte[7] = 0x00;
	canWrite(_ID, CommandPosition4Byte);
	// the servo will reply everytime we send command to it
	// readReplyFlush seem like to eliminate the tderror and rderror after sending command
	readReplyFlush(_ID);
	CANErrorCheck();

}




