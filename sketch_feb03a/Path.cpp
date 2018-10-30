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
	
	//�����������ƫ��
	angletoWorld =atan2(dx,dy);
			
	angletoWorld = angletoWorld/PI*180;
			
	//������Գ���ƫ��
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
		//#�Ƕ�������P
 		if (fabs(angletoCar)>50.00)
 		{
	 		if (angletoCar>0){angletoCar = 4;}
	 		if (angletoCar<0){angletoCar = -4;}
 		}
		else//�Ƕ����С��PD
		{
			angletoCarErr = angletoCar - pre_angletoCar;//��������ƫ�ǵĲ�ֵ
			if (angletoCar > 0){angular_vel_z = kp*angletoCar - kd*angletoCarErr;}
			if (angletoCar < 0){angular_vel_z = kp*angletoCar + kd*angletoCarErr;}
		}
		pre_angletoCar = angletoCar; //����ƫ��
		
		linear_vel_x = 0.2;
	}
	else
	{ 		
 		//#�Ƕ�������P
 		if (fabs(angletoCar)>50.00)
 		{
			if (angletoCar>0){angular_vel_z = 4;}
			if (angletoCar<0){angular_vel_z = -4;}
 		}
 		else//�Ƕ����С��PD
 		{
 			angletoCarErr = angletoCar - pre_angletoCar;//��������ƫ�ǵĲ�ֵ
			if (angletoCar > 0){angular_vel_z = kp*angletoCar - kd*angletoCarErr;}
 			if (angletoCar < 0){angular_vel_z = kp*angletoCar + kd*angletoCarErr;}
 		}
		pre_angletoCar = angletoCar; //����ƫ��
		
 		linear_vel_x = 1;
		
		#ifdef Path_DEBUG
		//��ʾ�����
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

//�Ƕ���
bool Path::rotatetoP(double presentP,double targetP)
{
	//������Գ���ƫ��
	angletoCar = CcltAngleSub(targetP, presentP);
	
	if (fabs(angletoCar)<5.00)
	{
		return true;
	}
	//#�Ƕ�������P
	if (fabs(angletoCar)>50.00)
	{
		if (angletoCar>0){angular_vel_z = 2;}
		if (angletoCar<0){angular_vel_z = -2;}
	}
	else//�Ƕ����С��PD
	{
		angletoCarErr = angletoCar - pre_angletoCar;//��������ƫ�ǵĲ�ֵ
		if (angletoCar > 0){angular_vel_z = kp*angletoCar - kd*angletoCarErr;}
		if (angletoCar < 0){angular_vel_z = kp*angletoCar + kd*angletoCarErr;}
	}
	pre_angletoCar = angletoCar; //����ƫ��
	
	linear_vel_x = 0;
	
	#ifdef Path_DEBUG
	//��ʾ�����
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
	//����λ�ò�
	double dx = targetX - presentX;
	double dy = targetY - presentY;
	
	//�����������ƫ��
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
	//������Գ���ƫ��
	angletoCar = CcltAngleSub(targetP, presentP);
	if (fabs(angletoCar)<1.00)
	{
		return true;
	}
	else//�Ƕ����С��PD
	{
		if (angletoCar > 0){angular_vel_z = 1;}
		if (angletoCar < 0){angular_vel_z = -1;}
	}	
	linear_vel_x = 0;
	
	#ifdef Path_DEBUG
	//��ʾ�����
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