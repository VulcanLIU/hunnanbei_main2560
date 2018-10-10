/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */

#include "Display.h"
//#include "SecondDisplay.h"

#include "ComwithPC.h"
#include "mMotor.h"
//#define POS_DEBUG
#include "mPOS.c"

#include "Timer1.h"
#include "Timer3.h"

#include "mPS2.h"
#define  PS2_DEBUG
#include "ComwithShot.h"
#include "Kinematics.h"
//#define Kinematics_DEBUG
#include "ComwithMotor.h"
//路径
#include "Path.h"
//任务
#include "tasks.cpp"
//传感器板
#include "SensorBoard.h"
//GY25
#include "ComwithGY25.h"
//#define GY25_DEBUG
//#define GO_DEBUG

//Beginning of Auto generated function prototypes by Atmel Studio
//End of Auto generated function prototypes by Atmel Studio
Display dp;
//SecondDisplay dp2;
ComwithPC toPC;
mMotor motor;
mPS2 PS2;

Timer1 tc1;
Timer3 tc3;

ComwithShot CS;
Kinematics kinematics(300,0.068, 0.5,8);
ComwithMotor cm;
Path path;
//传感器板
SensorBoard Sensors;
//GY25
//ComwithGY25 GY25;

String state = "begin";
// int point[41][2]={{0,0},{0,200},{0,400},{0,600},{0,800},{0,1000},{0,1210},{40,1360},{150,1470},{300,1510},{465,1510},
// {583,1550},{669,1627},{700,1745},{700,1945},{700,2145},{700,2345},{700,2545},{700,2675},
// {669,2825},{583,2879},{465,2910},{265,2910},{65,2910},{-265,2910},{-465,2910},{-583,2879},{-669,2825},{-700,2675},
// {-700,2475},{-700,2275},{-700,2075},{-700,1875},{-700,1745},{-669,1627},{-583,1550},{-465,1510},{-265,1510},{-124,1569},{0,1710},{0,1745}};
int point[34][2]={{0,0},{0,200},{0,400},{0,600},{0,800},{0,1000},
	{0,1200},{0,1400},{0,1610},{200,1610},{400,1610},{600,1610},
		{600,1810},{600,2010},{600,2210},{600,2410},{600,2610},
			{600,2810},{400,2810},{200,2810},{0,2810},{-200,2810},
				{-400,2810},{-600,2810},{-600,2610},{-600,2410},
					{-600,2210},{-600,2010},{-600,1810},{-600,1610},
						{-400,1610},{-200,1610},{0,1610},{0,1985}};

int i,j;
static int index=0;
int step = 0;

//time_out功能
int previous_time = 0;
bool has_time_out = false;

//PID time
int PID_pre_time = 0;

//任务列表
task task_list[3] = {SMALL_CIRCLE_WAKING,FIX_POSITION,DO_SHOT};
int present_task_index = 0;
task present_task = task_list[present_task_index];

void receiveEvent(int howMany);
void POS_refresh();

void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);
	toPC.begin(115200);
	
	dp.begin();
	cm.begin();
	POS_begin();
	
	tc1.setMode("CTC",50);
	tc1.attachInterrupt(POS_refresh);

	//GY25配置
	GY25.begin();
	GY25.singleMode();
	GY25.correctYaw();
	
	//PS2手柄
	PS2.begin();
	PS2.refresh();
	PS2.isRC = true;
	
	//申明自身的地址
	Wire.begin(8);
	Wire.onReceive(receiveEvent);
}

void loop() {
	// put your main code here, to run repeatedly:
	//PS2手柄遥控底盘
	
	PS2.refresh();
	Sensors.refresh();
	
	//陀螺仪刷新
	GY25.singleMode();
	
	toFenQiu();
	//询问收球模块收了几个
	askforFenQiu();
	//解算转速
	Kinematics::output pwm;
	float linear_vel_x = 0;
	float linear_vel_y = 0;
	float angular_vel_z = 0;
	
	if (!PS2.isRC)
	{
		//四个串联任务
		//#小圆行走
		if (present_task == SMALL_CIRCLE_WAKING)
		{
			if (index == 0){if (path.gotoPoint(x,y,p,0,1400)){index++;}}
			if (index == 1){if (path.rotatetoP(p,90)){index++;}}
			if (index == 2){if (path.gotoPoint(x,y,p,600,1400)){index++;}}
			if (index == 3){if (path.rotatetoP(p,0)){index++;}}
			if (index == 4){if (path.gotoPoint(x,y,p,600,2400)){index++;}}
			if (index == 5){if (path.rotatetoP(p,-90)){index++;}}
			if (index == 6){if (path.gotoPoint(x,y,p,-600,2400)){index++;}}
			if (index == 7){if (path.rotatetoP(p,-180)){index++;}}
			if (index == 8){if (path.gotoPoint(x,y,p,-600,1400)){index++;}}
			if (index == 9){if (path.rotatetoP(p,-180)){index++;}}
			if (index == 10){Serial.print("OjbK");}
			/*这里写任务具体内容	*/
			//  
			double target_X = point[index][0];
			double target_Y = point[index][1];
			
// 			bool arrived;
// 			
// 			if (path.rotatetoP(x,y,p,target_X,target_Y))
// 			{
// 				arrived = path.gotoPoint(x,y,p,target_X,target_Y);
// 			}
// 			else
// 			{
// 				arrived = false;
// 			}
			
			//path.rotatetoP(x,y,p,1000,0);
			
			#ifdef GO_DEBUG
			Serial.print("SMALL_CIRCLE_WAKING");
			Serial.print("	");
			Serial.print("TargetXY:");
			Serial.print(target_X);
			Serial.print(",");
			Serial.print(target_Y);
			Serial.print("	");
			Serial.print("NowXY:");
			Serial.print(x);
			Serial.print(",");
			Serial.println(y);	
			#endif
					
// 			if (arrived == true){index++;previous_time = millis();}
// 			else
// 			{
// 				if (millis()-previous_time >= 10000)
// 				{
// 					has_time_out = true;
// 					
// 					#ifdef GO_DEBUG
// 					Serial.println("Waring!I am in SMALL_CIRCLE_WAKING");
// 					#endif
// 				}
// 			}
			
			/*这里负责正常任务跳转*/
			if (index>=34){present_task = SMALL_CIRCLE_FIX_POSITION;index = 0;Serial.println("go to  SMALL_CIRCLE_FIX_POSITION!");}//此任务完成 开始走向
			
			/*这里负责异常任务跳转*/
			
		}
		
		//#小圆定位程序
		if (present_task == SMALL_CIRCLE_FIX_POSITION)
		{
			//后退动作
			if (fix_pos_list == MOVE_BACK)
			{
				linear_vel_x = -1;
				angular_vel_z = 0;
				if (Sensors.back_hit_wall == true){
					delay(1000);
					if (Sensors.back_hit_wall == true){
						linear_vel_y = 0;
						angular_vel_z = 0;
						fix_pos_list = USE_LASER_DATA;
					}
				}
			}
		}
		linear_vel_x = path.linear_vel_x;
		angular_vel_z = path.angular_vel_z;
	}
	else
	{
		linear_vel_x = float(map(PS2.analog_RY,0,255,+1000,-1000))/1000;
		linear_vel_y = 0;
		angular_vel_z =float(map(PS2.analog_LX,0,255,-3140,+3140))/1500;
		if (PS2.left == true){angular_vel_z =float(map(68,0,255,-3140,+3140))/1500;PS2.left = false;}
		if (PS2.right == true){angular_vel_z =float(map(188,0,255,-3140,+3140))/1500;PS2.right = false;}
		if (PS2.up == true){linear_vel_x =float(map(68,0,255,1000,-1000))/1000;PS2.up = false;}
		if (PS2.down == true){linear_vel_x =float(map(188,0,255,1000,-1000))/1000;PS2.down = false;}
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
	
	cm.SendAtoALL(pwm.motor1,-pwm.motor2,pwm.motor3,-pwm.motor4);
	
	//通过蓝牙发送给上位机数据
	//toPC.tellMotors(pwm.motor1,pwm.motor2,pwm.motor3,pwm.motor4);
	toPC.tellXYP(x,y,p);
	//toPC.tellState(state);
	
	//通过OLED显示器显示XYP
	dp.refresh(pwm.motor1,pwm.motor2,pwm.motor3,pwm.motor4,x,y,p,PS2.state,GY25.YPR[0],GY25.YPR[1],GY25.YPR[2]);
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
	
	#ifdef GY25_DEBUG
	Serial.print("Yaw:");
	Serial.println(GY25.YPR[0]);
	#endif // GY25_DEBUG
}

void receiveEvent(int howMany) {
	while (1 < Wire.available()) { // loop through all but the last
		int c = Wire.read(); // receive byte as a character
	}
}

void serialEvent3() {
	GY25.refresh();
}

double _p2;

void POS_refresh()
{
	x1 = (double)x_step*1.16/x_line*PI*wheel_d;x_step = 0;//行走的距离 单位mm；
	y1 = (double)y_step*1.16/y_line*PI*wheel_d;y_step = 0;//行走的距离 单位mm；
	p1 = (double)p_step*1.16/p_line*PI*wheel_d;p_step = 0;//行走的距离 单位mm；
	p2 = GY25.YPR[0]/180.00*PI;
	
	double dx = x1;
	double dy = y1;
	double dp = p1;
	
	double dy2 = (dy - dp)/2;
	double dp2 = p2 - _p2;
	if (dp2 >  PI/2)  dp2 -= PI;
	if (dp2 < -PI/2)  dp2 += PI;
	double dx2 = (dx - dp2*d2);
	_p2 = p2;
	
	/*//凯子算法
	double dax = dy*(cos(PI/2-dp_rad/2));
	double day = dy*(sin(PI/2-dp_rad/2));
	
	x+= dax*cos(p_rad)+day*sin(p_rad);
	y+= day*cos(p_rad)-dax*sin(p_rad);
	*/
	
	//刘展鹏的算法
	x2+= dx2*cos(p2)+dy2*sin(p2);
	y2+= dy2*cos(p2)-dx2*sin(p2);
	
	x = x2;
	y = y2;
	p = p2/PI*180;
}