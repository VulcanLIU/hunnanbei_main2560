/*
* Display.cpp
*
* Created: 2018/8/20 1:57:09
* Author: Vulcan
*/


#include "Display.h"
#include <Wire.h>
#include "Adafruit_SSD1306.h"
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
// default constructor
Display::Display()
{

}

void Display::begin()
{
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
	display.display();
	delay(1000);

	display.clearDisplay();
}

void Display::displayTime(int _time)
{
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0,5);
	display.clearDisplay();
	display.print("Time:");
	display.println(_time);
	display.display();
}

void Display::displayStr(String str)
{
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0,5);
	display.clearDisplay();
	display.print("Time:");
	display.println(str);
	display.display();
}

void Display::refresh(int L1_speed,int L2_speed,int R1_speed,int R2_speed,double pos_x,double pos_y,double pos_p,String state)
{
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0,0);
	display.clearDisplay();
	//��ʾϵͳ����ʱ��
	display.print("Time:");
	display.print(millis());
	
	//��ʾ����PWM
	display.setCursor(0,8);
	display.print("L1:");
	display.print(L1_speed);
	display.setCursor(64,8);
	display.print("R1:");
	display.print(R1_speed);
	
	display.setCursor(0,16);
	display.print("L2:");
	display.print(L2_speed);
	display.setCursor(64,16);
	display.print("R2:");
	display.print(R2_speed);
	
	display.setCursor(0,30);
	display.print("PS2:");
	display.print(state);
	display.display();
	
	//
}
