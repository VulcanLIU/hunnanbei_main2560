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
static const int INT0A = 19;
static const int INT0B = 4;
static const int INT1A = 2;
static const int INT1B = 5;
static const int INT2A = 3;
static const int INT2B = 6;

//物理信息
const double d = 200; //两轮间距160mm
const double d2 = 60;
const double wheel_d = 38;//轮直径38mm
const int x_line = 512;
const int y_line = 512;
const int p_line = 512;

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
	double x1 = (double)x_step/x_line*PI*wheel_d;//*1.74466;//行走的距离 单位mm；
	double y1 = (double)y_step/y_line*PI*wheel_d;//*1.74466;//行走的距离 单位mm；
	double p1 = (double)p_step/p_line*PI*wheel_d;//*1.74466;//行走的距离 单位mm；
	
	double y2 = (y1-p1)/2;
	double p2 = (y1+p1)/d;//弧度制rad
	double x2 = p2*d2+x1;//

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
	
	double dax = dy*(cos(PI/2-dp_rad/2));
	double day = dy*(sin(PI/2-dp_rad/2));
	
	x+= dax*cos(p_rad)+day*sin(p_rad);
	y+= day*cos(p_rad)-dax*sin(p_rad);
	
	p = p_rad/PI*180;
	
	#ifdef POS_DEBUG
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
	
	//原始数据——>原始长度信息
	Serial.print("X2_mm:");
	Serial.print(x2);
	Serial.print("Y2_mm:");
	Serial.print(y2);
	Serial.print("P2_mm:");
	Serial.print(p2 );  //串口显口
	
	//最终数据
	Serial.print("X_fina:");
	Serial.print(x);
	Serial.print("Y_fina:");
	Serial.print(y);
	Serial.print("P_fina:");
	Serial.println(p);  //串口显口
	#endif // SERIAL_DEBUG
}

