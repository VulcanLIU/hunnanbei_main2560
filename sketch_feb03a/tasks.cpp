/*
 * tasks.cpp
 *
 * Created: 2018/9/29 5:56:41
 *  Author: Vulcan
 */ 

//列举出机器人在场上所有执行出来的任务
enum task{
	SMALL_CIRCLE_WAKING,
	SMALL_CIRCLE_FIX_POSITION,
	FIX_POSITION,
	DO_SHOT,
	STAY,
	LARGE_CIRCLE_WAKING,
	LARGE_CIRCLE_FIX_POSITION
	};

enum fix_pos{
	DONOTHING,
	MOVE_BACK,
	CHECK_BACK_SENSOR,
	USE_LASER_DATA
	};

extern fix_pos fix_pos_list = MOVE_BACK;
//task present_task = SMALL_CIRCLE_WAKING;

//task task_list[5];
extern int value=-1;
void read_DeFenBall()
{
	pinMode(7,INPUT_PULLUP);
	value = digitalRead(7);
}

void toFenQiu()
{
	read_DeFenBall();
	
	//告诉分球部分分黑球还是白球
	Wire.beginTransmission(5);
	Wire.write(value);
	Wire.endTransmission();
	//Serial.print(value);
}

void askforFenQiu()
{
	 Wire.requestFrom(5, 1);    // request 6 bytes from slave device #8

	 while (Wire.available()) { // slave may send less than requested
		 int c = Wire.read(); // receive a byte as character
		 Serial.print("qiushu:");         // print the character
		 Serial.println(c);         // print the character
	 }
}