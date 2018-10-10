/*
* Timer3.cpp
*
* Created: 2018/6/18 18:14:58
* Author: Vulcan
*/

#include "Timer3.h"
#include <avr/interrupt.h>

#define F_TC3 F_CPU/_prescaler

static void nothing(void) {
}

//static volatile voidFuncPtr _intFunc = nothing;

static volatile voidFuncPtr _intFunc = nothing;

int Timer3::calculate(int _prescaler,int _time)
{
	int _OCR3A = _time*(F_CPU/1000)/_prescaler;
	oc = _OCR3A;
	return _OCR3A;
}

int Timer3::select_prescaler(int _time)
{
	int _prescaler_table[5] = {1,8,64,256,1024};
	int index = 0;
	int _MAX_time = 0;
	for (;index<5;index++)
	{
		_MAX_time = 65536*_prescaler_table[index]/(F_CPU/1000);
		this->MAX_time = _MAX_time;
		if (_MAX_time>_time)
		{
			break;
		}
		else if (index == 4)
		{
			# warning "The MAX_time of Timer3 cannot reach the requirement!"
		}
	}
	return _prescaler_table[index];
}

// default constructor
Timer3::Timer3()
{
	SREG |=0x80;//打开全局中断
} //Timer3

Timer3::Timer3(String _mode,int _time)
{
	
	setMode(_mode,_time);
	Timer3();
	# warning "Mode set completely!"
} //Timer3

void Timer3::setMode(String _mode,int _time)
{
	this->Mode = _mode;
	//自动选择分频因子
	this->prescaler = select_prescaler(_time);
	
	//模式选择
	_mode.toUpperCase();
	if (_mode == "CTC")
	{
		TCCR3A = 0X00;
		TCCR3B = 0X00;
		//确定CTC模式
		TCCR3B = (1<<WGM32);
		
		//配置预分频器
		if (prescaler == 1){TCCR3B |=(1<<CS10);}
		if (prescaler == 8){TCCR3B |=(1<<CS11);}
		if (prescaler == 64){TCCR3B |=(1<<CS11)|(1<<CS10);}
		if (prescaler == 256){TCCR3B |=(1<<CS12);}
		if (prescaler == 1024){TCCR3B |=(1<<CS12)|(1<<CS10);}
		
		//设置时间
		OCR3A = this->calculate(prescaler,_time);
		//配置匹配溢出中断
		TIMSK3 =(1<<OCIE3A);
	}
}

void Timer3::attachInterrupt(void (*userFunc)(void))
{
	_intFunc = userFunc;
}

// default destructor
Timer3::~Timer3()
{
} //~Timer3

ISR (TIMER3_COMPA_vect)
{
	_intFunc();
}