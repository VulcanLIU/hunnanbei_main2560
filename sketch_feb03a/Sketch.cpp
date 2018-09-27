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
int point[34][2]={{0,0},{0,200},{0,400},{0,600},{0,800},{0,1000},{0,1200},{0,1400},{0,1610},{200,1610},{400,1610},{600,1610},{600,1810},{600,2010},{600,2210},{600,2410},{600,2610},{600,2810},{400,2810},{200,2810},{0,2810},{-200,2810},{-400,2810},{-600,2810},{-600,2610},{-600,2410},{-600,2210},{-600,2010},{-600,1810},{-600,1610},{-400,1610},{-200,1610},{0,1610},{0,1985}};
int i,j;
static int index=0;
int step = 0;

void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);
	toPC.begin(115200);
	
	dp.begin();
	cm.begin();
	POS_begin();
	
	tc1.setMode("CTC",50);
	tc1.attachInterrupt(POS_refresh);
	PS2.begin();

	PS2.refresh();
	PS2.isRC = true;
}

void loop() {
	// put your main code here, to run repeatedly:
	//PS2手柄遥控底盘
	PS2.refresh();
// 	if (PS2.shot)
// 	{
// 		CS.SendXYPandSHOT(x,y,p);
// 		state = "Shot!!!!!";
// 		PS2.shot = false;
// 	}
// 	else
// 	{
// 		state = "Running!!!!";
// 	}
	
	//解算转速
	Kinematics::output pwm;
	float linear_vel_x = 0;
	float linear_vel_y = 0;
	float angular_vel_z = 0;
	
	if (!PS2.isRC)
	{
		if(path.gotoPoint(x,y,p,point[index][0],point[index][1]))
		{
			
			index++;
		}
		if(index<=34)
		{
			if(index==9)
			{
				//右转90度
			}
			if(index == 12||index == 18||index == 24||index == 30||index == 33)
			{
				//左转90度
			}
		}
		else index = 0;//一圈走完，回到起点，等待下次路径规划
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
	
	#ifdef POS_DEBUG
	Serial.print("X_step:");
	Serial.print(x_step);
	Serial.print(" Y_step:");
	Serial.print(y_step);
	Serial.print(" P_step:");
	Serial.print(p_step);
	
	//原始数据——>原始长度信息
	Serial.print("X_mm:");
	Serial.print(x1);
	Serial.print("Y_mm:");
	Serial.print(y1);
	Serial.print("P_mm:");
	Serial.print(p1);  //串口显口
	
	//原始数据——>原始长度信息
	Serial.print("X2_mm:");
	Serial.print(x2);
	Serial.print("Y2_mm:");
	Serial.print(y2);
	Serial.print("P2_mm:");
	Serial.print(p2);  //串口显口
	
	//最终数据
	Serial.print("X_fina:");
	Serial.print(x);
	Serial.print("Y_fina:");
	Serial.print(y);
	Serial.print("P_fina:");
	Serial.println(p);  //串口显口
	#endif // SERIAL_DEBUG
}