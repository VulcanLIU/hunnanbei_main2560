/* 
* ComwithShot.h
*
* Created: 2018/8/12 17:55:29
* Author: Vulcan
*/


#ifndef __COMWITHSHOT_H__
#define __COMWITHSHOT_H__


class ComwithShot
{
public:
	const int address = 5;//ÉäÇòÄ£¿éµÄµØÖ·
	
	bool connected = false;
	
	ComwithShot();
	void begin();
	void check();
	void SendXYPandSHOT(double,double,double);
}; //ComwithShot

#endif //__COMWITHSHOT_H__
