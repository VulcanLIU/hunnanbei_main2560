/*
* mPOS.cpp
*
* Created: 2018/8/20 18:27:53
* Author: Vulcan
*/
#include "Arduino.h"
#include "mPOS.h"
#include "ComwithGY25.h"
//#define POS_DEBUG

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

void POS_refresh()
{
	x1 = (double)x_step*1.16/x_line*PI*wheel_d;x_step = 0;//行走的距离 单位mm；
	y1 = (double)y_step*1.16/y_line*PI*wheel_d;y_step = 0;//行走的距离 单位mm；
	p1 = (double)p_step*1.16/p_line*PI*wheel_d;p_step = 0;//行走的距离 单位mm；
	p2 = GY25.YPR[0]/180.00*PI;
	
	double dx = x1;
	double dy = y1;
	double dp = p1;
	
	double dy2 = (dy - dp)/2;
	double dp2 = p2 - _p2;
	if (dp2 >  PI/2)  dp2 -= PI;
	if (dp2 < -PI/2)  dp2 += PI;
	double dx2 = (dx - dp2*d2);
	_p2 = p2;
	
	/*//凯子算法
	double dax = dy*(cos(PI/2-dp_rad/2));
	double day = dy*(sin(PI/2-dp_rad/2));
	
	x+= dax*cos(p_rad)+day*sin(p_rad);
	y+= day*cos(p_rad)-dax*sin(p_rad);
	*/
	
	//刘展鹏的算法
	x2+= dx2*cos(p2)+dy2*sin(p2);
	y2+= dy2*cos(p2)-dx2*sin(p2);
	
	x = x2;
	y = y2;
	p = p2/PI*180;
}