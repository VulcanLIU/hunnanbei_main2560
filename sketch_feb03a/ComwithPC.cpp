/* 
* ComwithPC.cpp
*
* Created: 2018/8/20 2:22:48
* Author: Vulcan
*/


#include "ComwithPC.h"

#include "HardwareSerial.h"
// default constructor
ComwithPC::ComwithPC()
{
} //ComwithPC

void ComwithPC::begin(int baud)
{
	Serial1.begin(115200);
	Serial1.println("OK!");
}

void ComwithPC::tellMotors(int speed_L,int dir_L,int speed_R,int dir_R)
{
	//告诉上位机电机的转速情况
	Serial1.print("Motors:");
	Serial1.print(speed_L);
	Serial1.print(',');
	Serial1.print(dir_L);
	Serial1.print(',');
	Serial1.print(speed_R);
	Serial1.print(',');
	Serial1.println(dir_R);
}

void ComwithPC::tellXYP(double x,double y,double p)
{
	Serial1.print("XYP:");
	Serial1.print(x);
	Serial1.print(',');
	Serial1.print(y);
	Serial1.print(',');
	Serial1.println(p);
}

void ComwithPC::tellState(String state)
{
	Serial1.print("State:");
	Serial1.println(state);
}

