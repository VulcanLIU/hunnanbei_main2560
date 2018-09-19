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
// 	//是否到达
// 	double distance = sqrt((x-pos[next_index][0])*(x-pos[next_index][0]) + (y-pos[next_index][1])*(y-pos[next_index][1]));
// 	//到达
// 	if (distance<=50)
// 	{
// 		next_index ++;
// 		if (next_index>=16)
// 		{
// 			next_index = 0;
// 		}
// 		return true;
// 	}
// 	//没有到达
// 	else
// 	{
// 		//计算两点偏角
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
* @param  	presentXYP：当前姿态；
targetXY：想要去的那个点；

* @retval 	返回值是是否已经到达这个点。如果是，返回true，调用者接收到返回值后需要换一个坐标点
*/

bool Path::gotoPoint(double presentX,double presentY,double presentP,double targetX,double targetY)
{
	
	//计算位置差
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
		//计算相对世界偏角
		angletoWorld =atan2(dy,dx);
		
		angletoWorld = angletoWorld/PI*180;
		
		//计算相对车的偏角
		//angletoCar = angletoWorld - presentP;
		angletoCar = CcltAngleSub(angletoWorld , presentP);
		
		angular_vel_z = k*angletoCar;
		
		#ifdef Path_DEBUG
		//显示距离差
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
	//计算位置差
	double dx = targetX - presentX;
	double dy = targetY - presentY;
	
	double distance = sqrt((dx)*(dx)+(dy)*(dy));

	//计算相对世界偏角
	angletoWorld =atan2(dy,dx);
	
	angletoWorld = angletoWorld/PI*180;
	
	//计算相对车的偏角
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
