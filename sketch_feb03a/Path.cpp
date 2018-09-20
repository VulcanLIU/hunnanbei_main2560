/*
* Path.cpp
*
* Created: 2018/9/7 1:49:05
* Author: Vulcan
*/


#include "Path.h"
#include <math.h>
#include "Arduino.h"
//#define Path_DEBUG
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
	if (distance < 20)
	{
		Serial.println("Arrived");
		linear_vel_x = 0;
		angular_vel_z = 0;
		return true;
	}
	else
	{
		//�����������ƫ��
		angletoWorld =atan2(dy,dx);
		
		angletoWorld = angletoWorld/PI*180;
		
		//������Գ���ƫ��
		//angletoCar = angletoWorld - presentP;
		angletoCar = CcltAngleSub(angletoWorld , presentP);
		
		angular_vel_z = k*angletoCar;
		linear_vel_x = 0.2;
		
		#ifdef Path_DEBUG
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

bool Path::gotoP(double presentX,double presentY,double presentP,double targetX,double targetY)
{
	//����λ�ò�
	double dx = targetX - presentX;
	double dy = targetY - presentY;
	
	double distance = sqrt((dx)*(dx)+(dy)*(dy));

	//�����������ƫ��
	angletoWorld =atan2(dy,dx);
	
	angletoWorld = angletoWorld/PI*180;
	
	//������Գ���ƫ��
	//angletoCar = angletoWorld - presentP;
	angletoCar = CcltAngleSub(angletoWorld , presentP);
	if (abs(angletoCar)<0.2)
	{
		return true;
	}
	else
	{
		angular_vel_z = k*angletoCar;
		return false;
	}
	
}

void Path::setK(float k)
{
	this->k = k;
}

double Path::CcltAngleSub(double minuend, double subtrahend)
{
	double result = 0.0f;
	result = minuend - subtrahend;
	if (result >  180.0f)  result -= 360.0f;
	if (result < -180.0f)  result += 360.0f;
	return result;
}
