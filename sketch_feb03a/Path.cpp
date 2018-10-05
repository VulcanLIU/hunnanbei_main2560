/*
* Path.cpp
*
* Created: 2018/9/7 1:49:05
* Author: Vulcan
*/


#include "Path.h"
#include <math.h>
#include "Arduino.h"
#define Path_DEBUG

struct PID{
	
	float erro;
	float last_erro;
	float p;
	float i;
	float d;
	float out;
	};
PID angle;
PID d_ang;


// default constructor
Path::Path()
{
} //Path

/**
* @name 	gotoPoint
* @brief
* @param  	presentXYP����ǰ��̬��
targetXY����Ҫȥ���Ǹ��㣻

* @retval 	����ֵ���Ƿ��Ѿ���������㡣����ǣ�����true�������߽��յ�����ֵ����Ҫ��һ�������
*/

bool Path::gotoPoint(double presentX,double presentY,double presentP,double targetX,double targetY)
{
	
	//����λ�ò�
	double dx = targetX - presentX;
	double dy = targetY - presentY;
	double distance = sqrt((dx)*(dx)+(dy)*(dy));
	static unsigned char counter;
	if (distance < 50)
	{
		Serial.println("Arrived");
		linear_vel_x = 0;
		angular_vel_z = 0;
		return true;
	}
	else
	{
		//�����������ƫ��
		angletoWorld =atan2(dx,dy);
 		
 		angletoWorld = angletoWorld/PI*180;
 		
 		//������Գ���ƫ��
		angletoCar = CcltAngleSub(angletoWorld , presentP);
 			//������������ֵ
 		double _angletoCar = angletoCar/180*PI;
 		
 		//#�Ƕ�������P
 		if (fabs(angletoCar)>10.00)
 		{
 			angular_vel_z = k*_angletoCar;
 		}
 		else//�Ƕ����С��PD
 		{
 			angletoCarErr = _angletoCar - pre_angletoCar;//��������ƫ�ǵĲ�ֵ
 			angular_vel_z = kp2*_angletoCar - kd2*angletoCarErr;
 		}
 		
 		pre_angletoCar = _angletoCar; //����ƫ��
 		linear_vel_x = 0.8;
		
		#ifndef Path_DEBUG
		//��ʾ�����
		Serial.print("X:");
		Serial.print(presentX);
		Serial.print("Y:");
		Serial.print(presentY);
		Serial.print("P:");
		Serial.print(presentP);
		Serial.print("distance:");
		Serial.print(distance);
		Serial.print("dx:");
		Serial.print(dx);
		Serial.print("dy:");
		Serial.print(dy);
		Serial.print("angletoWorld:");
		Serial.print(angletoWorld);
		Serial.print("angletoCar:");
		Serial.print(angletoCar);
		Serial.print("angular_vel_z:");
		Serial.print(angular_vel_z);
		Serial.println();
		#endif // Path_DEBUG
		return false;
	}
	

}

double Path::CcltAngleSub(double minuend, double subtrahend)
{
	double result = 0.0f;
	result = minuend - subtrahend;
	if (result >  180.0f)  result -= 360.0f;
	if (result < -180.0f)  result += 360.0f;
	return result;
}
