/*
 * tasks.h
 *
 * Created: 2018/10/11 3:27:14
 *  Author: Vulcan
 */ 


#ifndef TASKS_H_
#define TASKS_H_

#include "Arduino.h"
#include "Wire.h"

//列举出机器人在场上所有执行出来的任务
enum task{
	SMALL_CIRCLE_WAKING,
	SMALL_CIRCLE_FIX_POSITION,
	FIX_POSITION,
	DO_SHOT,
	STAY,
	MIDDLE_CIRCLE_WAKING,
	MIDDLE_CIRCLE_FIX_POSITION,
	LARGE_CIRCLE_WAKING,
	LARGE_CIRCLE_FIX_POSITION,
	GO_TO_CHUFAQU,
	DIAO_TOU,
	CHUFAQU_BACKHIT,
	_USE_LASER_DATA,
	DO_SHOT_TWO,
	};

enum fix_pos{
	DONOTHING,
	SLOW_ROTATE,
	MOVE_BACK,
	CHECK_BACK_SENSOR,
	USE_LASER_DATA
	};

extern fix_pos fix_pos_list;
extern task present_task;
extern int value;

void read_DeFenBall();
void toFenQiu();
void askforFenQiu();

extern double _p;

#endif /* TASKS_H_ */