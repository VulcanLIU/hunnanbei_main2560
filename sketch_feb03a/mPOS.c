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


double x2_previous = 0;
double y2_previous = 0;
double p2_previous = 0;

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
const int p_line = 256;

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
	x_step++;
	else
	x_step--;
}

void blinkY()
{
	int ystate = digitalRead(INT1B);
	if (ystate == HIGH)
	y_step--;
	else
	y_step++;
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

void POS_refresh()
{
	x1 = (double)x_step/x_line*PI*wheel_d;//行走的距离 单位mm；
	y1 = (double)y_step/y_line*PI*wheel_d;//行走的距离 单位mm；
	p1 = (double)p_step/p_line*PI*wheel_d;//*1.011;//行走的距离 单位mm；
	
	y2 = (y1-p1)/2;
	p2 = (y1+p1)/d;//弧度制rad
	x2 = -p2*d2+x1;//

	if (p2>=PI)
	{
		p2 = fmod((p2+PI),(2*PI))-PI;
	}
	if (p2<=(-PI))
	{
		p2 = fmod((p2-PI),(2*PI))+PI;
	}
	
	double dx = x2 - x2_previous;x2_previous = x2;
	double dy = y2 - y2_previous;y2_previous = y2;
	double dp_rad =p2 - p2_previous;p2_previous = p2;
	double p_rad = p2;
	
	/*//凯子算法
	double dax = dy*(cos(PI/2-dp_rad/2));
	double day = dy*(sin(PI/2-dp_rad/2));
	
	x+= dax*cos(p_rad)+day*sin(p_rad);
	y+= day*cos(p_rad)-dax*sin(p_rad);
	*/
	
	//刘展鹏的算法
	x+= dx*cos(p_rad)+dy*sin(p_rad);
	y+= dy*cos(p_rad)-dx*sin(p_rad);
	
	p = p_rad/PI*180;
}

