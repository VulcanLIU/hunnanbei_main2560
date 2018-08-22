/* 
* mMotor.h
*
* Created: 2018/8/20 18:46:16
* Author: Vulcan
*/


#ifndef __MMOTOR_H__
#define __MMOTOR_H__

#include "Arduino.h"

class mMotor
{
public:
	//应该使用const 对于硬件接线做出定义
	const uint8_t L_DIR_PIN = 2;
	const uint8_t L_PWM_PIN = 3;
	const uint8_t R_DIR_PIN = 4;
	const uint8_t R_PWM_PIN = 5;
	
	mMotor();
    void begin();//配置PWM端口
	void setPWM(int L_pwm,int R_pwm);//设置两轮转速
	void setVnW(int v,int w);
	
	//应存放两轮转速信息和方向信息
	int L_PWM = 0;
	int L_DIR = HIGH;
	int R_PWM = 0;
	int R_DIR = HIGH;
	
}; //mMotor

#endif //__MMOTOR_H__
