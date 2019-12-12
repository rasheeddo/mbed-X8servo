#ifndef __X8SERVO_HPP
#define __X8SERVO_HPP

class X8servo
{
	public:

		X8servo();

		void Int16ToByteData(unsigned int Data, unsigned char StoreByte[2]);

		void Int32ToByteData(unsigned int Data, unsigned char StoreByte[4]);

		uint16_t ByteDataToInt16(unsigned char ByteData[2]);

		int64_t ByteDataToInt64(unsigned char ByteData[8]);

		long map(long x, long in_min, long in_max, long out_min, long out_max);

		void CANErrorCheck();

		void canWrite(unsigned int _ID, char data[8]);

		void readReplyFlush(unsigned int _ID);

		bool readReply(unsigned int _ID, unsigned char output[8]);

		int64_t ReadMultiturn(unsigned int _ID);

		void ReadMotorStatusError(unsigned int _ID);

		void MotorOff(unsigned int _ID);

		void MotorStop(unsigned int _ID);
		
		void MotorRun(unsigned int _ID);

		void SpeedControl(unsigned int _ID, float DPS);

		void PositionControl1(unsigned int _ID, float Deg);

		void PositionControl2(unsigned int _ID, float Deg, float DPS);

		void PositionControl3(unsigned int _ID, float Deg, unsigned char Direction);

		void PositionControl4(unsigned int _ID, float Deg, float DPS, unsigned char Direction);


	private:

		unsigned int gearRatio = 6;


};

#endif