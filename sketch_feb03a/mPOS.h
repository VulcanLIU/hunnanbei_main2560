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
	void begin();//配置三个中断
	void refresh();//放置算法的地方 更新XYP
	
	double x = 2400;
	double y = 0;
	double p = 0;
private:
	double x_step = 0;
	double y_step = 0;
	double p_step = 0;
	
	/*中断服务程序*/
	void blinkX();
	void blinkY();
	void blinkP();
	
}; //mPOS

#endif //__MPOS_H__
