/* 
* ComwithGY25.cpp
*
* Created: 2018/10/8 19:30:32
* Author: Vulcan
*/


#include "ComwithGY25.h"
#include <HardwareSerial.h>

// default constructor
ComwithGY25::ComwithGY25()
{
} //ComwithGY25

void ComwithGY25::begin()
{
	Serial3.begin(115200);
	delay(3000);
	correctYaw();
	Serial3.write(0XA5);
	Serial3.write(0X52);    //初始化GY25,连续输出模式
}

void ComwithGY25::autoMode()
{
	Serial3.write(0XA5);
	Serial3.write(0X52);    //初始化GY25,连续输出模式
}

void ComwithGY25::autoModeASCII()
{
	Serial3.write(0XA5);
	Serial3.write(0X53);    //初始化GY25,连续输出模式
}

void ComwithGY25::singleMode()
{
	Serial3.write(0XA5);
	Serial3.write(0X51);    //校正模式，校正俯仰横滚角 0 度，需要保持水平时候发送
}

void ComwithGY25::correctPitchRow()
{
	Serial3.write(0XA5);
	Serial3.write(0X54);    //校正模式，校正俯仰横滚角 0 度，需要保持水平时候发送
}

void ComwithGY25::correctYaw()
{
	Serial3.write(0XA5);
	Serial3.write(0X55);    //校正模式，校正航向 0 度，航向任意角度清零
}

void ComwithGY25::refresh() {
	while (Serial3.available()) {
		Re_buf[counter]=(unsigned char)Serial3.read();
		if(counter==0&&Re_buf[0]!=0xAA) return;      // 检查帧头
		counter++;
		if(counter==8)                //接收到数据
		{
			counter=0;                 //重新赋值，准备下一帧数据的接收
			sign=1;
		}
	}
	if(sign == 1)
	{
		if(Re_buf[0]==0xAA && Re_buf[7]==0x55)        //检查帧头，帧尾
		{
			YPR[0]=(Re_buf[1]<<8|Re_buf[2])/-100.00;   //合成数据，去掉小数点后2位
			YPR[1]=(Re_buf[3]<<8|Re_buf[4])/100.00;
			YPR[2]=(Re_buf[5]<<8|Re_buf[6])/100.00;
		}
		sign = 0;
	}
}

ComwithGY25 GY25 = ComwithGY25();