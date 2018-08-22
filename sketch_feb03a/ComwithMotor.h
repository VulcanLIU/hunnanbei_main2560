/* 
* ComwithMotor.h
*
* Created: 2018/7/29 9:57:39
* Author: Vulcan
*/


#ifndef __COMWITHMOTOR_H__
#define __COMWITHMOTOR_H__
#include "Wire.h"

class ComwithMotor
{
public:
	
	enum Motor{L1=1,L2,R1,R2};
	int A[4] = {};
	int B[4] = {};
	double kp = 0;
	double ki = 0;
	double kd = 0;
	
	ComwithMotor();
	void begin();
	void check();
	int ReadBfrom(Motor);
	void SendAtoALL(int,int,int,int);
	void SendKtoALL(double,double,double);
	bool connectedRight();
	bool connectedtoL1Right();
	bool connectedtoL2Right();
	bool connectedtoR1Right();
	bool connectedtoR2Right();
	bool connected = false;
	bool connectedL1 = false;
	bool connectedL2 = false;
	bool connectedR1 = false;
	bool connectedR2 = false;
	
private:
	void getK();
	void SendAto(Motor,int);
	void SendKto(Motor,double,double,double);

	
}; //ComwithMotor

#endif //__COMWITHMOTOR_H__
