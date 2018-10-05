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
	LARGE__CIRCLE_WAKING,
	FIX_POSITION,
	DO_SHOT
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