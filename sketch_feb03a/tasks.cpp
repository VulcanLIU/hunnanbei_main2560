/*
 * tasks.cpp
 *
 * Created: 2018/9/29 5:56:41
 *  Author: Vulcan
 */ 

#include "tasks.h"

fix_pos fix_pos_list = MOVE_BACK;


//task present_task = SMALL_CIRCLE_WAKING;
task present_task = SMALL_CIRCLE_FIX_POSITION;
int value=-1;
double _p = 0;

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