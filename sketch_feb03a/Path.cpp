/*
* Path.cpp
*
* Created: 2018/9/7 1:49:05
* Author: Vulcan
*/


#include "Path.h"
#include <math.h>
#include "Arduino.h"
#include "EEPROM.h"

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
	
	//计算相对世界偏角
	angletoWorld =atan2(dx,dy);
			
	angletoWorld = angletoWorld/PI*180;
			
	//计算相对车的偏角
	angletoCar = CcltAngleSub(angletoWorld , presentP);
	
	if (distance < 5)
	{
		Serial.println("Arrived");
		linear_vel_x = 0;
		angular_vel_z = 0;
		return true;
	}
	else if (distance < 200)
	{
		//#角度误差大用P
 		if (fabs(angletoCar)>50.00)
 		{
	 		if (angletoCar>0){angletoCar = 4;}
	 		if (angletoCar<0){angletoCar = -4;}
 		}
		else//角度误差小用PD
		{
			angletoCarErr = angletoCar - pre_angletoCar;//计算两次偏角的差值
			if (angletoCar > 0){angular_vel_z = kp*angletoCar - kd*angletoCarErr;}
			if (angletoCar < 0){angular_vel_z = kp*angletoCar + kd*angletoCarErr;}
		}
		pre_angletoCar = angletoCar; //保存偏角
		
		linear_vel_x = 0.2;
	}
	else
	{ 		
 		//#角度误差大用P
 		if (fabs(angletoCar)>50.00)
 		{
			if (angletoCar>0){angular_vel_z = 4;}
			if (angletoCar<0){angular_vel_z = -4;}
 		}
 		else//角度误差小用PD
 		{
 			angletoCarErr = angletoCar - pre_angletoCar;//计算两次偏角的差值
			if (angletoCar > 0){angular_vel_z = kp*angletoCar - kd*angletoCarErr;}
 			if (angletoCar < 0){angular_vel_z = kp*angletoCar + kd*angletoCarErr;}
 		}
		pre_angletoCar = angletoCar; //保存偏角
		
 		linear_vel_x = 1;
		
		#ifdef Path_DEBUG
		//显示距离差
		Serial.print("XYP:");
		Serial.print(presentX);
		Serial.print("	");
		Serial.print(presentY);
		Serial.print("	");
		Serial.print(presentP);
		Serial.print("	");
		
		Serial.print("TXYP:");
		Serial.print(targetX);
		Serial.print("	");
		Serial.print(targetY);
		Serial.print("	");

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

//角度制
bool Path::rotatetoP(double presentP,double targetP)
{
	//计算相对车的偏角
	angletoCar = CcltAngleSub(targetP, presentP);
	
	if (fabs(angletoCar)<5.00)
	{
		return true;
	}
	//#角度误差大用P
	if (fabs(angletoCar)>50.00)
	{
		if (angletoCar>0){angular_vel_z = 2;}
		if (angletoCar<0){angular_vel_z = -2;}
	}
	else//角度误差小用PD
	{
		angletoCarErr = angletoCar - pre_angletoCar;//计算两次偏角的差值
		if (angletoCar > 0){angular_vel_z = kp*angletoCar - kd*angletoCarErr;}
		if (angletoCar < 0){angular_vel_z = kp*angletoCar + kd*angletoCarErr;}
	}
	pre_angletoCar = angletoCar; //保存偏角
	
	linear_vel_x = 0;
	
	#ifdef Path_DEBUG
	//显示距离差
	Serial.print(presentP);
	Serial.print("	");
			
	Serial.print("TP:");
	Serial.print(targetP);
	Serial.print("	");

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

bool Path::rotatetoP(double presentX,double presentY,double presentP,double targetX,double targetY)
{
	//计算位置差
	double dx = targetX - presentX;
	double dy = targetY - presentY;
	
	//计算相对世界偏角
	angletoWorld =atan2(dx,dy);
	
	angletoWorld = angletoWorld/PI*180;
	
	#ifdef Path_DEBUG
	Serial.print("XYP:");
	Serial.print(presentX);
	Serial.print("	");
	Serial.print(presentY);
	Serial.print("	");	
	#endif

	
	return rotatetoP(presentP,angletoWorld);
}

bool Path::slowrotatetoP(double presentP,double targetP)
{
	//计算相对车的偏角
	angletoCar = CcltAngleSub(targetP, presentP);
	if (fabs(angletoCar)<1.00)
	{
		return true;
	}
	else//角度误差小用PD
	{
		if (angletoCar > 0){angular_vel_z = 1;}
		if (angletoCar < 0){angular_vel_z = -1;}
	}	
	linear_vel_x = 0;
	
	#ifdef Path_DEBUG
	//显示距离差
	Serial.print(presentP);
	Serial.print("	");
	
	Serial.print("TP:");
	Serial.print(targetP);
	Serial.print("	");

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

void Path::p_incre()
{
	kp+=0.01;
}

void Path::p_decre()
{
	kp-=0.01;
}

void Path::d_incre()
{
	kd+=0.001;
}

void Path::d_decre()
{
	kd-=0.001;
}

void Path::pd_save()
{
	EEPROM.write(kp_addr,kp*100);
	EEPROM.write(kd_addr,kd*1000);
}

void Path::pd_read()
{
	kp = EEPROM.read(kp_addr)/100.00;
	kd = EEPROM.read(kd_addr)/1000.000;
}

double Path::CcltAngleSub(double minuend, double subtrahend)
{
	double result = 0.0f;
	result = minuend - subtrahend;
	if (result >  180.0f)  result -= 360.0f;
	if (result < -180.0f)  result += 360.0f;
	return result;
}

Path path = Path();