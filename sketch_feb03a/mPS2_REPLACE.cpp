/*
* mPS2.cpp
*
* Created: 2018/8/20 18:58:40
* Author: Vulcan
*/


#include "mPS2_REPLACE.h"
#include "PS2X_lib.h" //for v1.6

/******************************************************************
* set pins connected to PS2 controller:
*   - 1e column: original
*   - 2e colmun: Stef?
* replace pin numbers by the ones you use
******************************************************************/
#define PS2_DAT        13  //14
#define PS2_CMD        11  //15
#define PS2_SEL        10  //16
#define PS2_CLK        12  //17

/******************************************************************
* select modes of PS2 controller:
*   - pressures = analog reading of push-butttons
*   - rumble    = motor rumbling
* uncomment 1 of the lines for each mode selection
******************************************************************/
//#define pressures   true
#define pressures   false
//#define rumble      true
#define rumble      false

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning
//you must always either restart your Arduino after you connect the controller,
//or call config_gamepad(pins) again after connecting the controller.
// default constructor

mPS2::mPS2()
{
} //mPS2

void mPS2::begin()
{
	Serial3.begin(115200);
	
	delay(300);  //added delay to give wireless ps2 module some time to startup, before configuring it
	
	//CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
	
	//setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
	error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
	
	if(error == 0){
		Serial.print("Found Controller, configured successful ");
		Serial.print("pressures = ");
		if (pressures)
		Serial.println("true ");
		else
		Serial.println("false");
		Serial.print("rumble = ");
		if (rumble)
		Serial.println("true)");
		else
		Serial.println("false");
		Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
		Serial.println("holding L1 or R1 will print out the analog stick values.");
		Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
	}
	else if(error == 1)
	Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
	
	else if(error == 2)
	Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

	else if(error == 3)
	Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
	
	//  Serial.print(ps2x.Analog(1), HEX);
	
	type = ps2x.readType();
	switch(type) {
		case 0:
		Serial.print("Unknown Controller type found ");
		break;
		case 1:
		Serial.print("DualShock Controller found ");
		break;
		case 2:
		Serial.print("GuitarHero Controller found ");
		break;
		case 3:
		Serial.print("Wireless Sony DualShock Controller found ");
		break;
	}
}

void mPS2::refresh()
{
	while (Serial3.available() > 0)
	{
		char c = Serial3.read();
		
		if (c == '\n')
		{
			int pos_AD = this->buff.indexOf(',');
			String str_analog_RY = buff.substring(0,pos_AD);
			analog_RY = str_analog_RY.toInt();
			
			buff.remove(0,pos_AD+1);
			
			String str_analog_LX = this->buff;
			analog_LX = str_analog_LX.toInt();
			
			//Serial.print("str_analog_RY:");
			//Serial.print(str_analog_RY);
			//Serial.print("str_analog_LX:");
			//Serial.println(str_analog_LX);
			
			buff = "";
		}
		else
		{
			buff += c;
		}		
	}
}