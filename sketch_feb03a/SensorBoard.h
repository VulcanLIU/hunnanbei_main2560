/* 
* SensorBoard.h
*
* Created: 2018/10/2 13:41:03
* Author: Vulcan
*/


#ifndef __SENSORBOARD_H__
#define __SENSORBOARD_H__

#include <stdint.h>

class SensorBoard
{
public:
	SensorBoard();
	
	void refresh();
	
	//���Թ�ƨ���Ƿ�ײǽ
	bool back_hit_wall = false;
	
	//�������������ݺ��ɼ����������ĽǶȺ�����
	double R1_laser = 0;
	double R2_laser = 0;
	double L1_laser = 0;
	double L2_laser = 0;
	double B_laser = 0;
	
	double laser_X = 0;
	double laser_Y = 0;
	double laser_P = 0;
	
	//mpu6050 ���ٶ�
	double ax = 0.0;
	double ay = 0.0;
	double axy = 0.0;
	
	//�����ɫ�������ɼ�����ɫ�Լ��ϳɳ�������ɫ
	uint8_t R = 0;
	uint8_t G = 0;
	uint8_t B = 0;
	uint32_t RGB = 0;
	
	//��ų��ر�׼ɫ
	const uint8_t BLUE_Zone_R = 0;
	const uint8_t BLUE_Zone_G = 0;
	const uint8_t BLUE_Zone_B = 255;
	
	const uint8_t RED_Zone_R = 255;
	const uint8_t RED_Zone_G = 0;
	const uint8_t RED_Zone_B = 0;
	
	const uint8_t Orange_Zone_R = 255;
	const uint8_t Orange_Zone_G = 128;
	const uint8_t Orange_Zone_B = 0;
	
    const uint8_t GREEN_Zone_R = 0;
	const uint8_t GREEN_Zone_G = 100;
	const uint8_t GREEN_Zone_B = 0;
}; //SensorBoard

#endif //__SENSORBOARD_H__
