/*
* mPOS.h
*
* Created: 2018/8/20 18:27:53
* Author: Vulcan
*/


#ifndef __MPOS_H__
#define __MPOS_H__

#include "Arduino.h"

void POS_begin();//配置三个中断
void POS_refresh();//放置算法的地方 更新XYP

double x = 2400;//中点位置
double y = 0;
double p = 0;

double x_step = 0;
double y_step = 0;
double p_step = 0;


double x2_previous = 0;
double y2_previous = 0;
double p2_previous = 0;

//接线信息
static const int INT0A = 19;
static const int INT0B = 4;
static const int INT1A = 18;
static const int INT1B = 3;
static const int INT2A = 2;
static const int INT2B = 4;

//物理信息
const double d = 160; //两轮间距160mm
const double wheel_d = 38;//轮直径38mm
const int x_line = 512;
const int y_line = 512;
const int p_line = 512;

/*中断服务程序*/
void blinkX();
void blinkY();
void blinkP();

#endif //__MPOS_H__
