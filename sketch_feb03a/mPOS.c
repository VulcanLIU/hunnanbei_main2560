/*
* mPOS.cpp
*
* Created: 2018/8/20 18:27:53
* Author: Vulcan
*/
#include "Arduino.h"
#define SERIAL_DEBUG
double x = 2400;//中点位置
double y = 0;
double p = 0;

double x_step = 0;
double y_step = 0;
double p_step = 0;


double x2_previous = 0;
double y2_previous = 0;
double p2_previous = 0;

//接线信息
static const int INT0A = 19;
static const int INT0B = 4;
static const int INT1A = 2;
static const int INT1B = 5;
static const int INT2A = 3;
static const int INT2B = 6;

//物理信息
const double d = 160; //两轮间距160mm
const double wheel_d = 38;//轮直径38mm
const int x_line = 512;
const int y_line = 512;
const int p_line = 512;

void blinkX()
{
	int xstate = digitalRead(INT0B);
	if (xstate == HIGH)
	x_step--;
	else
	x_step++;
}

void blinkY()
{
	int ystate = digitalRead(INT1B);
	if (ystate == HIGH)
	y_step++;
	else
	y_step--;
}

void blinkP()
{
	int pstate = digitalRead(INT2B);
	if (pstate == HIGH)
	p_step--;
	else
	p_step++;
}

void POS_begin()
{
// 	pinMode(INT0A, INPUT);
// 	pinMode(INT1A, INPUT);
// 	pinMode(INT2A, INPUT);
// 	
// 	pinMode(INT0B, INPUT);
// 	pinMode(INT1B, INPUT);
// 	pinMode(INT2B, INPUT);
// 	
// 	attachInterrupt(digitalPinToInterrupt(INT0A), blinkX, FALLING);
// 	attachInterrupt(digitalPinToInterrupt(INT1A), blinkY, FALLING);
// 	attachInterrupt(digitalPinToInterrupt(INT2A), blinkP, FALLING);
}

void POS_refresh()
{
	double x1 = (double)x_step/x_line*PI*wheel_d;//行走的距离 单位mm；
	double y1 = (double)y_step/y_line*PI*wheel_d;//行走的距离 单位mm；
	double p1 = (double)p_step/p_line*PI*wheel_d;//行走的距离 单位mm；
	
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
	Serial.println(p1);  //串口显口
	#endif // SERIAL_DEBUG
}

