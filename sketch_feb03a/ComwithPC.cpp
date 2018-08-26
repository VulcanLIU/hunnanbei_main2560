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
	Serial3.begin(115200);
	Serial3.println("OK!");
}

void ComwithPC::tellMotors(int speed_L,int dir_L,int speed_R,int dir_R)
{
	//告诉上位机电机的转速情况
	Serial3.print("Motors:");
	Serial3.print(speed_L);
	Serial3.print(',');
	Serial3.print(dir_L);
	Serial3.print(',');
	Serial3.print(speed_R);
	Serial3.print(',');
	Serial3.println(dir_R);
}

void ComwithPC::tellXYP(double x,double y,double p)
{
	Serial3.print("XYP:");
	Serial3.print(x);
	Serial3.print(',');
	Serial3.print(y);
	Serial3.print(',');
	Serial3.println(p);
}

void ComwithPC::tellState(String state)
{
	Serial3.print("State:");
	Serial3.println(state);
}

