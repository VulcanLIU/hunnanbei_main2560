/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */
#include "Display.h"
#include "ComwithPC.h"
#include "mMotor.h"
//#define POS_DEBUG
#include "mPOS.c"

#include "Timer1.h"
#include "mPS2.h"
//#define  PS2_DEBUG
#include "ComwithShot.h"
#include "Kinematics.h"
//#define Kinematics_DEBUG
#include "ComwithMotor.h"
#define Path_DEBUG
#include "Path.h"

//Beginning of Auto generated function prototypes by Atmel Studio
//End of Auto generated function prototypes by Atmel Studio
Display dp;
ComwithPC toPC;
mMotor motor;
mPS2 PS2;
Timer1 tc1;
ComwithShot CS;
Kinematics kinematics(300,0.068, 0.5,8);
ComwithMotor cm;
Path path;
String state = "begin";

int step = 0;

void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);
	toPC.begin(115200);
	
	dp.begin();
	cm.begin();
	POS_begin();
	
	tc1.setMode("CTC",20);
	tc1.attachInterrupt(POS_refresh);
	PS2.begin();

	PS2.refresh();
	PS2.isRC = true;
}

void loop() {
	// put your main code here, to run repeatedly:
	//PS2手柄遥控底盘
	PS2.refresh();
	if (PS2.shot)
	{
		CS.SendXYPandSHOT(x,y,p);
		state = "Shot!!!!!";
		PS2.shot = false;
	}
	else
	{
		state = "Running!!!!";
	}
	
	//解算转速
	Kinematics::output pwm;
	float linear_vel_x =  0;
	float linear_vel_y = 0;
	float angular_vel_z = 0;
	
	if (!PS2.isRC)
	{
		path.gotoPoint(x,y,p,500,500);
		linear_vel_x = path.linear_vel_x;
		angular_vel_z = path.angular_vel_z;
	}
	else
	{
		linear_vel_x =   float(map(PS2.analog_RY,0,255,1000,-1000))/1000;
		linear_vel_y = 0;
		angular_vel_z =float(map(PS2.analog_LX,0,255,3140,-3140))/1500;
	}
	pwm = kinematics.getPWM(linear_vel_x, linear_vel_y, angular_vel_z);
	
	// 	Serial2.print("linear_vel_x:");
	// 	Serial2.print(linear_vel_x);
	// 	Serial2.print("angular_vel_z:");
	// 	Serial2.print(angular_vel_z);
	
	#ifdef Kinematics_DEBUG
	Serial.print("pwm_motor1:");
	Serial.print(pwm.motor1);
	Serial.print("pwm_motor2:");
	Serial.print(pwm.motor2);
	Serial.print("pwm_motor3:");
	Serial.print(pwm.motor3);
	Serial.print("pwm_motor4:");
	Serial.println(pwm.motor4);
	#endif
	
	cm.SendAtoALL(pwm.motor1,pwm.motor2,pwm.motor3,pwm.motor4);
	
	//通过蓝牙发送给上位机数据
	toPC.tellMotors(pwm.motor1,pwm.motor2,pwm.motor3,pwm.motor4);
	toPC.tellXYP(x,y,p);
	toPC.tellState(state);
	
	//通过OLED显示器显示XYP
	dp.refresh(pwm.motor1,pwm.motor2,pwm.motor3,pwm.motor4,x,y,p,PS2.state);
	PS2.state = "";
}