/*
* Timer3.h
*
* Created: 2018/6/18 18:14:58
* Author: Vulcan
*/


#ifndef __TIMER3_H__
#define __TIMER3_H__
#include "WString.h"
#include "wiring_private.h"
//#define F_CPU 16000000UL

class Timer3
{
//variables
public:
	String Mode = "";
	int prescaler = 1;
	int MAX_time = 0;
	int oc = 0;
protected:
	
private:
	
//functions
public:
	Timer3();
	Timer3(String _mode,int _time);
	void setMode(String _mode,int _time);
	void attachInterrupt(void (*userFunc)(void));
	~Timer3();
protected:
private:
	int calculate(int _prescaler,int _time);
	int select_prescaler(int _time);
}; //Timer3

#endif //__Timer3_H__