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
// default constructor
Path::Path()
{
} //Path

// bool Path::arrivedPoint(double x,double y,double p)
// {
// 	//�Ƿ񵽴�
// 	double distance = sqrt((x-pos[next_index][0])*(x-pos[next_index][0]) + (y-pos[next_index][1])*(y-pos[next_index][1]));
// 	//����
// 	if (distance<=50)
// 	{
// 		next_index ++;
// 		if (next_index>=16)
// 		{
// 			next_index = 0;
// 		}
// 		return true;
// 	}
// 	//û�е���
// 	else
// 	{
// 		//��������ƫ��
// 		double dx = pos[next_index][0] - x;
// 		double dy = pos[next_index][1] - y;
//
// 		double da = - tanh(dy/dx);
// 		angular_vel_z = 0.1*da;
//
// 	}
// }
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
		
		#ifdef Path_DEBUG
		//��ʾ�����
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
