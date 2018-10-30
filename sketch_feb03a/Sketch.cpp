/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>
/*End of auto generated code by Atmel studio */

//#debug开关
//#define POS_DEBUG
//#define PS2_DEBUG
//#define Kinematics_DEBUG
//#define GY25_DEBUG
#define GO_DEBUG
//#define Sensorboard_DEBUG
//#define STA_BUTTON_DEBUG
//#define PD_DEBUG
#define CIRCLE_PATH

#include "Display.h"
#include "ComwithPC.h"
#include "mMotor.h"
#include "mPOS.c"
#include "Timer1.h"
#include "Timer3.h"
#include "mPS2.h"
#include "ComwithShot.h"
#include "Kinematics.h"
#include "ComwithMotor.h"
#include "Path.h"
#include "SensorBoard.h"
#include "ComwithGY25.h"
#include "tasks.h"
//Beginning of Auto generated function prototypes by Atmel Studio
//End of Auto generated function prototypes by Atmel Studio
Display dp;
ComwithPC toPC;
mMotor motor;
mPS2 PS2;
Timer1 tc1;
Timer3 tc3;
Kinematics kinematics(280,0.068, 0.5,8);
ComwithMotor cm;
//Path path;
SensorBoard Sensors;

String state = "begin";
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

float linear_vel_x = 0;
float linear_vel_y = 0;
float angular_vel_z = 0;

//time_out功能
int previous_time = 0;
bool has_time_out = false;

//PID time
int PID_pre_time = 0;

void receiveEvent(int howMany);
void POS_refresh();
void task_refresh();
void start();

void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);
	toPC.begin(115200);
	
	dp.begin();
	cm.begin();
	POS_begin();
	
	tc1.setMode("CTC",50);
	tc1.attachInterrupt(POS_refresh);
	tc3.setMode("CTC",100);
	tc3.attachInterrupt(task_refresh);
	
	//GY25配置
	GY25.begin();
	GY25.singleMode();
	GY25.correctYaw();
	
	//PS2手柄
	PS2.begin();
	PS2.refresh();
	PS2.isRC = true;
	//手柄选择位
	pinMode(9,INPUT_PULLUP);

	//申明自身的地址
	Wire.begin(8);
	Wire.setClock(400000);
	Wire.onReceive(receiveEvent);

	//启动按钮
	pinMode(18, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(18), start, FALLING);
	
	//
	path.pd_read();
}

void loop() {
	// put your main code here, to run repeatedly:
	
	//#PS2手柄遥控底盘
	
	if (digitalRead(9) == LOW)
	{
		PS2.refresh();
	}
	
	//#传感器板更新
	//Sensors.refresh();
	Sensors.getInfo();
	#ifdef Sensorboard_DEBUG
	Serial.print("R_cm:");
	Serial.print(Sensors.R1_laser);
	Serial.print("	L_cm:");
	Serial.print(Sensors.L1_laser);
	Serial.print("	B_cm:");
	Serial.print(Sensors.B_laser);
	Serial.print("	value1:");
	Serial.print(Sensors.value1);
	Serial.print("	value2:");
	Serial.println(Sensors.value2);
	#endif
	
	//#陀螺仪刷新
	GY25.singleMode();
	
	//#沟通分球
	toFenQiu();
	//询问收球模块收了几个
	askforFenQiu();
	
	//#解算转速
	Kinematics::output pwm;
	
	if (!PS2.isRC)
	{
		//#小圆射球程序
		if (present_task == DO_SHOT)
		{
			int dx = x;
			int dy = y - 1600;
			int dis = sqrt(dx*dx+dy*dy);
						
			#ifdef GO_DEBUG
			Serial.print("DO_SHOT");
			Serial.print("	");
			Serial.print("x:");
			Serial.print(x);
			Serial.print("y:");
			Serial.print(y);
			Serial.print("dis:");
			Serial.println(dis);
			#endif

			if (dis<1000)
			{
				CS.SendXYPandSHOT(x,y,p);
				CS.StartMillis = millis();
				present_task = STAY;
			}
			else
			{
				present_task = MIDDLE_CIRCLE_WAKING;
			}
			CS.SendXYPandSHOT(x,y,p);
		}
		//#小圆等待
		if (present_task == STAY)
		{
			#ifdef GO_DEBUG
			Serial.print("STAY!!!");
			Serial.print("	");
			Serial.print("CS.StartMillis:");
			Serial.println(CS.StartMillis);
			Serial.print("Millis:");
			Serial.println(millis());
			#endif
			if (millis() - CS.StartMillis>25000){present_task = MIDDLE_CIRCLE_WAKING;}
		}
	}
	else
	{
		linear_vel_x = float(map(PS2.analog_RY,0,255,+1000,-1000))/1000;
		linear_vel_y = 0;
		angular_vel_z =float(map(PS2.analog_LX,0,255,-3140,+3140))/1500;
	}

	pwm = kinematics.getPWM(linear_vel_x, linear_vel_y, angular_vel_z);
	
	#ifdef Kinematics_DEBUG
	Serial.print("linear_vel_x");
	Serial.print(linear_vel_x);
	Serial.print("angular_vel_z");
	Serial.print(angular_vel_z);
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
	
	#ifdef PD_DEBUG
	Serial.print("KP:");
	Serial.print(path.kp);
	Serial.print("KD");
	Serial.print(path.kd*10);
	Serial.println();  //串口显口
	#endif
}

void receiveEvent(int howMany) {
	while (1 < Wire.available()) { // loop through all but the last
		int c = Wire.read(); // receive byte as a character
	}
}

void serialEvent3() {
	GY25.refresh();
}

void start()
{
	PS2.isRC = !PS2.isRC;
	#ifdef STA_BUTTON_DEBUG
	Serial.println("Start!");
	#endif //STA_BUTTON_DEBUG
	GY25.correctYaw();
	delay(200);
}

void task_refresh()
{
	if (!PS2.isRC)
	{
		//四个串联任务
		//#小圆行走
		if (present_task == SMALL_CIRCLE_WAKING)
		{
			/*这里写任务具体内容	*/
			#ifdef CIRCLE_PATH
				if (index == 0){if (path.gotoPoint(x,y,p,0,1400)){index++;}}
				if (index == 1){if (path.rotatetoP(p,90)){index++;}}
				if (index == 2){if (path.gotoPoint(x,y,p,573,1637)){index++;}}
				if (index == 3){if (path.gotoPoint(x,y,p,810,2210)){index++;}}
				if (index == 4){if (path.gotoPoint(x,y,p,573,2782)){index++;}}
				if (index == 5){if (path.gotoPoint(x,y,p,0,3020)){index++;}}
				if (index == 6){if (path.gotoPoint(x,y,p,-573,2782)){index++;}}		
				if (index == 7){if (path.gotoPoint(x,y,p,-810,2210)){index++;}}	
				if (index == 8){if (path.gotoPoint(x,y,p,-573,1637)){index++;}}
				if (index == 9){if (path.gotoPoint(x,y,p,-100,1350)){index++;}}	
				if (index == 10){if (path.rotatetoP(p,-179)){index++;};}
				if (index == 11){index++;}
			#else
				if (index == 0){if (path.gotoPoint(x,y,p,0,1400)){index++;}}
				if (index == 1){if (path.rotatetoP(p,90)){index++;}}
				if (index == 2){if (path.gotoPoint(x,y,p,600,1400)){index++;}}
				if (index == 3){if (path.rotatetoP(p,0)){index++;}}
				if (index == 4){if (path.gotoPoint(x,y,p,600,2850)){index++;}}
				if (index == 5){if (path.rotatetoP(p,-90)){index++;}}
				if (index == 6){if (path.gotoPoint(x,y,p,-600,2850)){index++;}}
				if (index == 7){if (path.rotatetoP(p,-179)){index++;}}
				if (index == 8){if (path.gotoPoint(x,y,p,-600,1400)){index++;}}
				if (index == 9){if (path.rotatetoP(p,90)){index++;}}
				if (index == 10){if (path.gotoPoint(x,y,p,-125,1400)){index++;}}
				if (index == 11){if (path.rotatetoP(p,179)){index++;}}
			#endif
			
			#ifdef GO_DEBUG
			Serial.print("SMALL_CIRCLE_WAKING");
			Serial.print("	");
			#endif
			
			/*这里负责正常任务跳转*/
			if (index>=12){present_task = SMALL_CIRCLE_FIX_POSITION;index = 0;Serial.println("go to  SMALL_CIRCLE_FIX_POSITION!");tc3.counter = 0;}//此任务完成 开始走向
			
			/*这里负责异常任务跳转*/
		}
		
		//#小圆定位程序
		if (present_task == SMALL_CIRCLE_FIX_POSITION){
			//后退动作
			#ifdef GO_DEBUG
			Serial.println("SMALL_CIRCLE_FIX_POSITION");
			Serial.print("	");
			#endif
			if(fix_pos_list == SLOW_ROTATE)
			{
				#ifdef GO_DEBUG
				Serial.print("SLOW_ROTATE");
				Serial.print("	");
				#endif		
				
				if (path.slowrotatetoP(p,179))
				{
					tc3.counter++;
					if (tc3.counter >= 4 )
					{
						fix_pos_list = MOVE_BACK;
					}
				}
					
			}
			if (fix_pos_list == MOVE_BACK){
				
				#ifdef GO_DEBUG
				Serial.print("MOVE_BACK");
				Serial.print("	");
				#endif
				
				path.linear_vel_x = -1;
				path.angular_vel_z = 0;
				
				if (Sensors.back_hit_wall == true)
				{
					Serial.print("HIT_BACK!");
					Serial.print("	");
					path.linear_vel_x =  0;
					path.angular_vel_z = 0;
					
					fix_pos_list = USE_LASER_DATA;
				}
			}
			if (fix_pos_list == USE_LASER_DATA&&y<=2000){
// 				x2 = Sensors.L1_laser;
// 				y2 = 1700;
				present_task = DO_SHOT;
			}
		}
		//#中圆行走
		if (present_task == MIDDLE_CIRCLE_WAKING)
		{
			#ifdef GO_DEBUG
			Serial.print(" MIDDLE_CIRCLE_WAKING");
			Serial.print("	");
			#endif
			if (index == 0){if (path.gotoPoint(x,y,p,0,1200)){index++;}}
			if (index == 1){if (path.rotatetoP(p,90)){index++;}}
			if (index == 2){if (path.gotoPoint(x,y,p,714,1496)){index++;}}
			if (index == 3){if (path.gotoPoint(x,y,p,1010,2210)){index++;}}
			if (index == 4){if (path.gotoPoint(x,y,p,714,2924)){index++;}}
			if (index == 5){if (path.gotoPoint(x,y,p,0,3220)){index++;}}
			if (index == 6){if (path.gotoPoint(x,y,p,-714,2924)){index++;}}
            if (index == 7){if (path.gotoPoint(x,y,p,-1010,2210)){index++;}}
			if (index == 8){if (path.gotoPoint(x,y,p,-714,1496)){index++;}}
			if (index == 9){if (path.gotoPoint(x,y,p,-75,1250)){index++;}}
			if (index == 10){if (path.rotatetoP(p,-179)){index++;};}
			if (index == 11){index++;}
			//待改
			if (index >= 12){present_task = SMALL_CIRCLE_WAKING;index = 0;Serial.println("go to  SMALL_CIRCLE_FIX_POSITION!");tc3.counter = 0;}
		}
		//#中圆定位
		if (present_task == MIDDLE_CIRCLE_FIX_POSITION)
		{
			//后退动作
			#ifdef GO_DEBUG
			Serial.println("MIDDLE_CIRCLE_FIX_POSITION");
			Serial.print("	");
			#endif
			if (fix_pos_list == MOVE_BACK){
				
				#ifdef GO_DEBUG
				Serial.print("MOVE_BACK");
				Serial.print("	");
				#endif
				
				path.linear_vel_x = -1;
				path.angular_vel_z = 0;
				
				if (Sensors.back_hit_wall == true){
					delay(300);
					if (Sensors.back_hit_wall == true){
						
						#ifdef GO_DEBUG
						Serial.print("HIT_BACK");
						Serial.print("	");
						#endif
						
						path.linear_vel_x = 0;
						path.angular_vel_z = 0;
						fix_pos_list = USE_LASER_DATA;
					}
				}
			}
		}
		//#去出发区
		if (present_task == GO_TO_CHUFAQU)
		{
			path.linear_vel_x = 1;
			path.angular_vel_z = -0.5;
			if (Sensors.area != 0 && Sensors.B_laser >= 1500)
			{
				present_task = CHUFAQU_BACKHIT;
				_p = path.CcltAngleSub(p,180);
			}
		}
		// 		//#掉头
		// 		if (present_task == DIAO_TOU){if (path.rotatetoP(p,_p)){present_task = ;}}
		//#屁股怼墙
		if (present_task == CHUFAQU_BACKHIT){
			path.linear_vel_x = -1;
			path.angular_vel_z = 0;

			if (Sensors.back_hit_wall == true){
					#ifdef GO_DEBUG
					Serial.print("HIT_BACK");
					Serial.print("	");
					#endif
					
					path.linear_vel_x = 0;
					path.angular_vel_z = 0;
					present_task = _USE_LASER_DATA;
			}
		}
		//#使用激光数据
		if (present_task == _USE_LASER_DATA)
		{
// 			x2 = Sensors.L1_laser;
// 			y2 = 2000;
			present_task = DO_SHOT_TWO;
		}
		//#两边射球
		if(present_task == DO_SHOT_TWO)
		{
			#ifdef GO_DEBUG
			Serial.print("DO_SHOT");
			Serial.print("	");
			#endif
			CS.begin();
			CS.SendXYPandSHOT(x,y,p);
			CS.StartMillis = millis();
			present_task = STAY;
		}
		
		linear_vel_x = path.linear_vel_x;
		angular_vel_z = path.angular_vel_z;
	}
}