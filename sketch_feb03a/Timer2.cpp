/* 
* Timer2.cpp
*
* Created: 2018/10/5 16:00:10
* Author: Vulcan
*/


#include "Timer2.h"
#include <avr/interrupt.h>

#define F_TC2 F_CPU/_prescaler

static void nothing(void) {
}
tccr
//static volatile voidFuncPtr _intFunc = nothing;

static volatile voidFuncPtr _intFunc = nothing;

//
int Timer2::calculate(int _prescaler,int _time)
{
	int _OCR2 = _time*(F_CPU/1000)/_prescaler;
	oc = _OCR2;
	return _OCR2;
}

//选择预分频器
int Timer2::select_prescaler(int _time)
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
			# warning "The MAX_time of Timer2 cannot reach the requirement!"
		}
	}
	return _prescaler_table[index];
}

// default constructor
Timer2::Timer2()
{
	SREG |=0x80;//打开全局中断
} //Timer1

Timer2::Timer2(String _mode,int _time)
{
	setMode(_mode,_time);
	Timer2();
	# warning "Mode set completely!"
} //Timer1

void Timer2::setMode(String _mode,int _time)
{
	this->Mode = _mode;
	//自动选择分频因子
	this->prescaler = select_prescaler(_time);
	
	//模式选择
	_mode.toUpperCase();
	if (_mode == "CTC")
	{
		TCCR2 = 0X00;
		//确定CTC模式
		TCCR2 = (1<<WGM21);
		
		//配置预分频器
		if (prescaler == 1){TCCR1B |=(1<<CS10);}
		if (prescaler == 8){TCCR1B |=(1<<CS11);}
		if (prescaler == 64){TCCR1B |=(1<<CS11)|(1<<CS10);}
		if (prescaler == 256){TCCR1B |=(1<<CS12);}
		if (prescaler == 1024){TCCR1B |=(1<<CS12)|(1<<CS10);}
		
		//设置时间
		OCR1A = this->calculate(prescaler,_time);
		//配置匹配溢出中断
		TIMSK1 =(1<<OCIE1A);
	}
}

void Timer2::attachInterrupt(void (*userFunc)(void))
{
	_intFunc = userFunc;
}

// default destructor
Timer1::~Timer1()
{
} //~Timer1

ISR (TIMER1_COMPA_vect)
{
	_intFunc();
}
