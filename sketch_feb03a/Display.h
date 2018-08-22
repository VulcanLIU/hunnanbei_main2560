/* 
* Display.h
*
* Created: 2018/8/20 1:57:09
* Author: Vulcan
*/


#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "WString.h"

class Display
{
public:
	Display();
	void begin();
	void displayTime(int);
	void displayStr(String str);
	void refresh(int L1_speed,int L2_speed,int R1_speed,int R2_speed,double pos_x,double pos_y,double pos_p,String state);
protected:
private:
}; //Display

#endif //__DISPLAY_H__
