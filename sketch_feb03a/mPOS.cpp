/*
* mPOS.cpp
*
* Created: 2018/8/20 18:27:53
* Author: Vulcan
*/


#include "mPOS.h"

// default constructor
mPOS::mPOS()
{
} //mPOS

void mPOS::begin()
{
	
}

void blinkX();
void blinkY();
void blinkP();

void POS_begin()
{
	pinMode(mPOS::INT0B, INPUT);
	pinMode(mPOS::INT1B, INPUT);
	pinMode(mPOS::INT2B, INPUT);
	
	attachInterrupt(digitalPinToInterrupt(mPOS::INT0A), blinkX, FALLING);
	attachInterrupt(digitalPinToInterrupt(mPOS::INT1A), blinkY, FALLING);
	attachInterrupt(digitalPinToInterrupt(mPOS::INT2A), blinkP, FALLING);
}
void mPOS::refresh()
{
	double x1 = double(x)/x_line*PI*wheel_d;//行走的距离 单位mm；
	double y1 = double(y)/y_line*PI*wheel_d;//行走的距离 单位mm；
	double p1 = double(p)/p_line*PI*wheel_d;//行走的距离 单位mm；
	
	double x2 = x1;
	double y2 = (y1+p1)/2;
	double p2 = (y1 - p1)/d;//弧度制rad
	
	double dx = x2 - x2_previous;
	double dy = y2 - y2_previous;
	double p_rad =p2;
	
	x+= dx*cos(p_rad)+dy*sin(p_rad);
	y+= dy*cos(p_rad)-dx*sin(p_rad);
	p = p_rad/PI*180;
	
	#ifdef SERIAL_DEBUG
	//原始数据
	Serial.print("X_step:");
	Serial.print(x_step);
	Serial.print(" Y_step:");
	Serial.print(y_step);
	Serial.print(" P_step:");
	Serial.print(p_step);
	
	//原始数据——>原始长度信息
	Serial.print("X_mm:");
	Serial.print(x1);
	Serial.print("Y_mm:");
	Serial.print(y1);
	Serial.print("P_mm:");
	Serial.print(p1);  //串口显口
	
	//
	#endif // SERIAL_DEBUG
}
	double x_step = 0;
	double y_step = 0;
	double p_step = 0;
	
void blinkX()
{
	int xstate = digitalRead(mPOS::INT0B);
	if (xstate == HIGH)
	x_step--;
	else
	x_step++;
}

void blinkY()
{
	int ystate = digitalRead(mPOS::INT1B);
	if (ystate == HIGH)
	y_step++;
	else
	y_step--;
}

void blinkP()
{
	int pstate = digitalRead(mPOS::INT2B);
	if (pstate == HIGH)
	p_step--;
	else
	p_step++;
}

void mPOS::blinkX()
{
	int xstate = digitalRead(INT0B);
	if (xstate == HIGH)
	x_step--;
	else
	x_step++;
}

void mPOS::blinkY()
{
	int ystate = digitalRead(INT1B);
	if (ystate == HIGH)
	y_step++;
	else
	y_step--;
}

void mPOS::blinkP()
{
	int pstate = digitalRead(INT2B);
	if (pstate == HIGH)
	p_step--;
	else
	p_step++;
}
