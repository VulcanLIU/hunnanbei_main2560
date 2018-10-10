/*
* mPOS.cpp
*
* Created: 2018/8/20 18:27:53
* Author: Vulcan
*/
#include "Arduino.h"
//#define POS_DEBUG
double x = 0;//中点位置
double y = 0;
double p = 0;

long int x_step = 0;
long int y_step = 0;
long int p_step = 0;


double x1_previous = 0;
double y1_previous = 0;
double p1_previous = 0;

//接线信息
static const int INT0A = 19;//INT2
static const int INT0B = 4;
static const int INT1A = 2;//INT4
static const int INT1B = 5;
static const int INT2A = 3;//INT5
static const int INT2B = 6;

//物理信息
const double d = 200; //两轮间距160mm
const double d2 = 80;
const double wheel_d = 38;//轮直径38mm
const int x_line = 512;
const int y_line = 512;
const int p_line = 512;

//用于串口输出
double x1 = 0;
double y1 = 0;
double p1 = 0;

double x2 = 0;
double y2 = 0;
double p2 = 0;

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
	p_step++;
	else
	p_step--;
}

void POS_begin()
{
	pinMode(INT0A, INPUT_PULLUP);
	pinMode(INT1A, INPUT_PULLUP);
	pinMode(INT2A, INPUT_PULLUP);
	
	pinMode(INT0B, INPUT_PULLUP);
	pinMode(INT1B, INPUT_PULLUP);
	pinMode(INT2B, INPUT_PULLUP);
	
	attachInterrupt(digitalPinToInterrupt(INT0A), blinkX, FALLING);
	attachInterrupt(digitalPinToInterrupt(INT1A), blinkY, FALLING);
	attachInterrupt(digitalPinToInterrupt(INT2A), blinkP, FALLING);
}

void POS_clear()
{
	x_step = 0;
	y_step = 0;
	p_step = 0;
	
	x = 0;
	y = 0;
	p = 0;
}
