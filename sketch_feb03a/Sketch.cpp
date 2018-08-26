/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */
#include "Display.h"
#include "ComwithPC.h"
#include "mMotor.h"
#include "mPOS.c"
#include "Timer1.h"
#include "mPS2_REPLACE.h"
#include "ComwithShot.h"
#include "Kinematics.h"
#include "ComwithMotor.h"
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
String state = "begin";


void setup() {
	// put your setup code here, to run once:
	Serial.begin(9600);
	//toPC.begin(115200);
	dp.begin();
	cm.begin();
	POS_begin();
		pinMode(INT0A, INPUT_PULLUP);
		pinMode(INT1A, INPUT_PULLUP);
		pinMode(INT2A, INPUT_PULLUP);
		
		pinMode(INT0B, INPUT);
		pinMode(INT1B, INPUT);
		pinMode(INT2B, INPUT);
		
		attachInterrupt(digitalPinToInterrupt(INT0A), blinkX, FALLING);
		attachInterrupt(digitalPinToInterrupt(INT1A), blinkY, FALLING);
		attachInterrupt(digitalPinToInterrupt(INT2A), blinkP, FALLING);
	tc1.setMode("CTC",100);
	tc1.attachInterrupt(POS_refresh);
	PS2.begin();

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
	float linear_vel_x =   float(map(PS2.analog_RY,0,255,-1000,1000))/1000;
	float linear_vel_y = 0;
	float angular_vel_z =float(map(PS2.analog_LX,-128,+128,-3140,+3140))/2000;
	pwm = kinematics.getPWM(linear_vel_x, linear_vel_y, angular_vel_z);
	
// 	Serial.print("linear_vel_x:");
// 	Serial.print(linear_vel_x);
// 	Serial.print("angular_vel_z:");
// 	Serial.println(angular_vel_z);
	
	cm.SendAtoALL(pwm.motor1,pwm.motor2,pwm.motor3,pwm.motor4);
	
	//通过蓝牙发送给上位机数据
	//toPC.tellMotors(motor.L_PWM,motor.L_DIR,motor.R_PWM,motor.R_DIR);
	//toPC.tellXYP(x,y,p);
	//toPC.tellState(state);
	
	//通过OLED显示器显示XYP
	dp.refresh(pwm.motor1,pwm.motor2,pwm.motor3,pwm.motor4,x,y,p,PS2.state);
	PS2.state = "";
}