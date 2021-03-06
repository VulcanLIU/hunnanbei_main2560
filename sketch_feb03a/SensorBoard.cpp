/* 
* SensorBoard.cpp
*
* Created: 2018/10/2 13:41:03
* Author: Vulcan
*/


#include "SensorBoard.h"
#include "Wire.h"
#include "HardwareSerial.h"
// default constructor
SensorBoard::SensorBoard()
{
} //SensorBoard

void SensorBoard::refresh()
{
	//让传感器刷新
	Wire.beginTransmission(8);
	Wire.write(8);
	Wire.endTransmission();
}

void SensorBoard::getInfo()
{
	Wire.begin();
	Wire.requestFrom(8,9);
	
	while(Wire.available()){
		R_laser_h = Wire.read();
		R_laser_l = Wire.read();
		L_laser_h = Wire.read();
		L_laser_l = Wire.read();
		B_laser_h = Wire.read();
		B_laser_l = Wire.read();
		value1 = Wire.read();
		value2 = Wire.read();
		area = Wire.read();

		R1_laser = (R_laser_h<<8|R_laser_l);
		L1_laser = (L_laser_h<<8|L_laser_l);
		B_laser =  (B_laser_h<<8|B_laser_l);
		
		if (value2==0){back_hit_wall = true;}
		else{back_hit_wall = false;}
		}//等待数据
		
		Serial.print("value1:");
		Serial.print(value1);
		Serial.print("value2:");
		Serial.print(value2);
		Serial.print("back_hit_wall:");
		Serial.println(back_hit_wall);
}
