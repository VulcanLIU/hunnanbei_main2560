/* 
* mPOS.h
*
* Created: 2018/8/20 18:27:53
* Author: Vulcan
*/


#ifndef __MPOS_H__
#define __MPOS_H__


class mPOS
{
public:
	mPOS();
	void begin();//���������ж�
	void refresh();//�����㷨�ĵط� ����XYP
	
	double x = 2400;
	double y = 0;
	double p = 0;
private:
	double x_step = 0;
	double y_step = 0;
	double p_step = 0;
	
	/*�жϷ������*/
	void blinkX();
	void blinkY();
	void blinkP();
	
}; //mPOS

#endif //__MPOS_H__
