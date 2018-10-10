/*
* Timer2.h
*
* Created: 2018/10/5 16:00:11
* Author: Vulcan
*/


#ifndef __TIMER2_H__
#define __TIMER2_H__

#include "WString.h"
#include "wiring_private.h"

class Timer2
{
//variables
public:
	String Mode = "";
	int prescaler = 1;
	int MAX_time = 0;
	int oc = 0;

protected:

private:

public:
	Timer2();
	Timer2(String _mode,int _time);
	void setMode(String _mode,int _time);
	void attachInterrupt(void (*userFunc)(void));
	~Timer2();
	
protected:
private:
	int calculate(int _prescaler,int _time);
	int select_prescaler(int _time);
}; //Timer2

#endif //__TIMER2_H__
