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
		//计算相对世界偏角
		angletoWorld =atan2(dx,dy);
 		
 		angletoWorld = angletoWorld/PI*180;
 		
 		//计算相对车的偏角
		angletoCar = CcltAngleSub(angletoWorld , presentP);
 			//计算两次误差差值
 		double _angletoCar = angletoCar/180*PI;
 		
 		//#角度误差大用P
 		if (fabs(angletoCar)>10.00)
 		{
 			angular_vel_z = k*_angletoCar;
 		}
 		else//角度误差小用PD
 		{
 			angletoCarErr = _angletoCar - pre_angletoCar;//计算两次偏角的差值
 			angular_vel_z = kp2*_angletoCar - kd2*angletoCarErr;
 		}
 		
 		pre_angletoCar = _angletoCar; //保存偏角
 		linear_vel_x = 0.8;
		
		#ifndef Path_DEBUG
		//显示距离差
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
