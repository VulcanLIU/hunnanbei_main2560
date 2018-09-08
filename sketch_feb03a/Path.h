/* 
* Path.h
*
* Created: 2018/9/7 1:49:05
* Author: Vulcan
*/


#ifndef __PATH_H__
#define __PATH_H__


class Path
{
public:
	Path();
	int pos[20][2] = {{0,0},{1150,0},
					{2300,0},{2300,1150},{2300,2300},{2300,3450},
					{2300,4600},{1150,4600},{0,4600},{-1150,4600},{-2300,4600},
					{-2300,3450},{-2300,2300},{-2300,1150},{-2300,0},{-1150,0}};
						
	bool arrivedPoint(double x,double y,double p);
	
	//
	bool gotoPoint(double presentX,double presentY,double presentP,double targetX,double targetY);
	void setK(float k);
	float k = 0;
	
	//
	double CcltAngleSub(double minuend, double subtrahend);
	double angletoWorld = 0;
	float angular_vel_z = 0;
	float linear_vel_x = 0.8;
	int next_index = 1;
}; //Path

#endif //__PATH_H__
