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

void ComwithPC::begin(unsigned long int baud)
{
	Serial2.begin(baud);
	Serial2.println("OK!");
}

void ComwithPC::tellMotors(int speed_L,int dir_L,int speed_R,int dir_R)
{
	//告诉上位机电机的转速情况
	Serial2.print("Motors:");
	Serial2.print(speed_L);
	Serial2.print(',');
	Serial2.print(dir_L);
	Serial2.print(',');
	Serial2.print(speed_R);
	Serial2.print(',');
	Serial2.println(dir_R);
}

void ComwithPC::tellXYP(double x,double y,double p)
{
	Serial2.print("A:");
	Serial2.print(0);
	Serial2.print("B:");
	Serial2.print(0);
	Serial2.print("x:");
	Serial2.print(x);
	Serial2.print("y:");
	Serial2.print(y);
	Serial2.print("p:");
	Serial2.println(p);
// 	 	Serial2.print("XYP:");
// 	 	Serial2.print(x);
// 	 	Serial2.print(',');
// 	 	Serial2.print(y);
// 	 	Serial2.print(',');
// 	 	Serial2.println(p);
}

void ComwithPC::tellState(String state)
{
	Serial2.print("State:");
	Serial2.println(state);
}

