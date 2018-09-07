/* 
* Path.cpp
*
* Created: 2018/9/7 1:49:05
* Author: Vulcan
*/


#include "Path.h"
#include <math.h>
// default constructor
Path::Path()
{
} //Path

bool Path::arrivedPoint(double x,double y,double p)
{
	//是否到达
	double distance = sqrt((x-pos[next_index][0])*(x-pos[next_index][0]) + (y-pos[next_index][1])*(y-pos[next_index][1]));
	//到达
	if (distance<=100)
	{
		next_index ++;
		if (next_index>=16)
		{
			next_index = 0;
		}
		return true;
	}
	//没有到达
	else
	{
		//计算两点偏角
		double dx = pos[next_index][0] - x;
		double dy = pos[next_index][1] - y;
		
		double da = - tanh(dy/dx);
		angular_vel_z = 0.1*da;
		
	}
}
