/*
* mPOS.h
*
* Created: 2018/8/20 18:27:53
* Author: Vulcan
*/


#ifndef __MPOS_H__
#define __MPOS_H__

#include "Arduino.h"

void POS_begin();//���������ж�
void POS_refresh();//�����㷨�ĵط� ����XYP

double x = 2400;//�е�λ��
double y = 0;
double p = 0;

double x_step = 0;
double y_step = 0;
double p_step = 0;


double x2_previous = 0;
double y2_previous = 0;
double p2_previous = 0;

//������Ϣ
static const int INT0A = 19;
static const int INT0B = 4;
static const int INT1A = 18;
static const int INT1B = 3;
static const int INT2A = 2;
static const int INT2B = 4;

//������Ϣ
const double d = 160; //���ּ��160mm
const double wheel_d = 38;//��ֱ��38mm
const int x_line = 512;
const int y_line = 512;
const int p_line = 512;

/*�жϷ������*/
void blinkX();
void blinkY();
void blinkP();

#endif //__MPOS_H__
