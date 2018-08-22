/*
* mMotor.cpp
*
* Created: 2018/8/20 18:46:16
* Author: Vulcan
*/


#include "mMotor.h"

// default constructor
mMotor::mMotor()
{
} //mMotor

void mMotor::begin()
{
	pinMode(L_DIR_PIN,OUTPUT);
	pinMode(R_DIR_PIN,OUTPUT);
	pinMode(L_PWM_PIN,OUTPUT);
	pinMode(R_PWM_PIN,OUTPUT);
}

void mMotor::setPWM(int L_pwm,int R_pwm)
{
	digitalWrite(L_DIR_PIN,L_DIR);
	digitalWrite(R_DIR_PIN,R_DIR);
	analogWrite(L_PWM_PIN,L_PWM);
	analogWrite(R_PWM_PIN,R_PWM);
}

void mMotor::setVnW(int v,int w)
{
	L_PWM = v + w;
	R_PWM = v - w;

	if (L_PWM > 1024)
	{
		R_PWM -= L_PWM-1024;
		L_PWM = 1024;
		L_DIR = HIGH;
	}
	if (R_PWM > 1024)
	{
		L_PWM -= R_PWM-1024;
		R_PWM = 1024;
		R_DIR = HIGH;
	}
	if (L_PWM< 0)
	{
		L_PWM = -L_PWM;
		L_DIR = LOW;
	}
	if (R_PWM< 0)
	{
		R_PWM = -R_PWM;
		R_DIR = LOW;
	}
	setPWM(1,1);
}
