/*
* mPS2.h
*
* Created: 2018/8/20 18:58:40
* Author: Vulcan
*/


#ifndef __MPS2_H__
#define __MPS2_H__

#include "Arduino.h"

class mPS2
{
	public:
	mPS2();
	void begin();
	void refresh();
	int error = 0;
	byte type = 0;
	byte vibrate = 0;
	
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	
	bool shot_right = false;
	bool shot_left = false;
	bool shot = false;
	
	int analog_LY = 127;
	int analog_LX = 127;
	int analog_RY = 127;
	int analog_RX = 127;
	
	String state = "";
	String buff = "";
}; //mPS2

#endif //__MPS2_H__
