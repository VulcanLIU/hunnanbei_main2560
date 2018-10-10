/*
* ComwithPC.h
*
* Created: 2018/8/20 2:22:48
* Author: Vulcan
*/


#ifndef __COMWITHPC_H__
#define __COMWITHPC_H__

/*Serial2������*/
#include "Arduino.h"
class ComwithPC
{
	public:
	ComwithPC();
	void begin(unsigned long int baud);//���ò�����
	
	void tellMotors(int speed_L,int dir_L,int speed_R,int dir_R);//������ʾ��һ�����������
	
	void tellXYP(double,double,double);//��ʾXYP
	
	void tellState(String state);//��ʾ״̬��Ϣ
}; //ComwithPC

#endif //__COMWITHPC_H__
