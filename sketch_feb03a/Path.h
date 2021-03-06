/* 
* Path.h
*
* Created: 2018/9/7 1:49:05
* Author: Vulcan
*/


#ifndef __PATH_H__
#define __PATH_H__

#define Path_DEBUG

class Path
{
public:
	Path();
	
	//目标点数组
	int pos[20][2] = {{0,0},{1150,0},
					{2300,0},{2300,1150},{2300,2300},{2300,3450},
					{2300,4600},{1150,4600},{0,4600},{-1150,4600},{-2300,4600},
					{-2300,3450},{-2300,2300},{-2300,1150},{-2300,0},{-1150,0}};
					
	//运动到目标点的函数
	bool gotoPoint(double presentX,double presentY,double presentP,double targetX,double targetY);
	bool rotatetoP(double presentP,double targetP);
	bool rotatetoP(double presentX,double presentY,double presentP,double targetX,double targetY);
	bool slowrotatetoP(double presentP,double targetP);
	//P&PD 环节的K值
	
	int kp_addr = 0;
	int kd_addr = 1;
	
	void p_incre();
	void p_decre();
	
	void d_incre();
	void d_decre();
	
	void pd_save();
	void pd_read();
	
	float kp = 0.5;
	float kd = 0.005;
	
	//
	double CcltAngleSub(double minuend, double subtrahend);	//计算偏角专用函数
	
	double angletoWorld = 0;	//		 目标点相对赛场的偏角
	double angletoCar = 0;		//比例环：目标点相对车运动方向的偏角
	double angletoCarErr = 0;	//微分环：两次偏角的差值
	double pre_angletoCar = 0;
	
	//解算出来的运动参数
	float angular_vel_z = 0;
	float linear_vel_x = 0.2;
	
	int next_index = 1;
}; //Path
extern Path path;
#endif //__PATH_H__
