/* 
* ComwithShot.cpp
*
* Created: 2018/8/12 17:55:28
* Author: Vulcan
*/


#include "ComwithShot.h"
#include "HardwareSerial.h"
#include "Wire.h"
// default constructor
ComwithShot::ComwithShot()
{
	
} //ComwithShot

void ComwithShot::begin()
{
	Wire.begin();
	Serial.println("ComwithShot begin!");
	
	Wire.beginTransmission(address);
	Wire.println("begin:");//发送给射球模块的时候 射球模块复位 摩擦轮电机启动
	Wire.endTransmission();
}

void ComwithShot::check()
{
	Serial.println("ComwithShot check!");
	
	Wire.begin();
	Wire.requestFrom(address,1);
	int c = -1;
	if(Wire.available())
	{
		 c = Wire.read();
	}
	
	if ( c== address)
	{
		connected = true;
	}
	else
	{
		connected = false;
	}
}

void ComwithShot::SendXYPandSHOT(double x,double y,double p)
{
	Serial.print("ComwithShot");
	Serial.print("X:");
	Serial.print(x);
	Serial.print("Y:");
	Serial.print(y);
	Serial.print("P:");
	Serial.print(p);
	Serial.println("Fire");
	
	Wire.beginTransmission(address);
	Wire.print("XYP:");
	Wire.print("X:");
	Wire.print(x);
	Wire.print("Y:");
	Wire.print(y);
	Wire.print("P:");
	Wire.print(p);
	Wire.println("SHOT");
	Wire.endTransmission();
}

ComwithShot CS =  ComwithShot();