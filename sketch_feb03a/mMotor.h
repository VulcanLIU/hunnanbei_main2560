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
	//Ӧ��ʹ��const ����Ӳ��������������
	const uint8_t L_DIR_PIN = 2;
	const uint8_t L_PWM_PIN = 3;
	const uint8_t R_DIR_PIN = 4;
	const uint8_t R_PWM_PIN = 5;
	
	mMotor();
    void begin();//����PWM�˿�
	void setPWM(int L_pwm,int R_pwm);//��������ת��
	void setVnW(int v,int w);
	
	//Ӧ�������ת����Ϣ�ͷ�����Ϣ
	int L_PWM = 0;
	int L_DIR = HIGH;
	int R_PWM = 0;
	int R_DIR = HIGH;
	
}; //mMotor

#endif //__MMOTOR_H__
