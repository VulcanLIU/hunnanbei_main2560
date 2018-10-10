/*
* ComwithPC.h
*
* Created: 2018/8/20 2:22:48
* Author: Vulcan
*/


#ifndef __COMWITHPC_H__
#define __COMWITHPC_H__

/*Serial2接蓝牙*/
#include "Arduino.h"
class ComwithPC
{
	public:
	ComwithPC();
	void begin(unsigned long int baud);//设置波特率
	
	void tellMotors(int speed_L,int dir_L,int speed_R,int dir_R);//用来显示哪一个电机掉线了
	
	void tellXYP(double,double,double);//显示XYP
	
	void tellState(String state);//显示状态信息
}; //ComwithPC

#endif //__COMWITHPC_H__
