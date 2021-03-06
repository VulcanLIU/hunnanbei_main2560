/*
* mPS2.cpp
*
* Created: 2018/8/20 18:58:40
* Author: Vulcan
*/


#include "mPS2.h"
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
	Serial.begin(115200);
	
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
	if(type == 2){ //Guitar Hero Controller
	}
	else { //DualShock Controller
		ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed
		
		if(ps2x.Button(PSB_START))
		{
			Serial.println("Start is being held");
			state = "Start is being held";
		}
		//will be TRUE as long as button is pressed

		
		if(ps2x.Button(PSB_SELECT)){Serial.println("Select is being held");state="Select is being held";}
			
		if(ps2x.Button(PSB_PAD_UP)) {      //will be TRUE as long as button is pressed
			Serial.print("Up held this hard: ");
			Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
			up = true;
			state = "up is being held";
		}
		if(ps2x.Button(PSB_PAD_RIGHT)){
			Serial.print("Right held this hard: ");
			Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
			right = true;
			state = "Right is being held";
		}
		if(ps2x.Button(PSB_PAD_LEFT)){
			Serial.print("LEFT held this hard: ");
			Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
			left = true;
			state = "LEFT is being held";
		}
		if(ps2x.Button(PSB_PAD_DOWN)){
			Serial.print("DOWN held this hard: ");
			Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
			down = true;
			state = "DOWN is being held";
		}

		vibrate = ps2x.Analog(PSAB_CROSS);  //this will set the large motor vibrate speed based on how hard you press the blue (X) button
		if (ps2x.NewButtonState()) {        //will be TRUE if any button changes state (on to off, or off to on)
			if(ps2x.Button(PSB_L3)){Serial.println("L3 pressed");state = "L3 is being held";}
			
			if(ps2x.Button(PSB_R3)){Serial.println("R3 pressed");state = "R3 is being held";}

			if(ps2x.Button(PSB_L2)){Serial.println("L2 pressed");state = "L2 is being held";}
			
			if(ps2x.Button(PSB_R2)){Serial.println("R2 pressed");state = "R2 is being held";}

			if(ps2x.Button(PSB_TRIANGLE)){Serial.println("Triangle pressed");state = "Triangle is being held";}

		}

		if(ps2x.ButtonPressed(PSB_CIRCLE)){Serial.println("Circle just pressed");shot_left = true;state = "Circle is being held";}
		
		if(ps2x.NewButtonState(PSB_CROSS)){Serial.println("X just changed");shot = true;state = "X is being held";}
		
		if(ps2x.ButtonReleased(PSB_SQUARE)){Serial.println("Square just released");shot_left = true;state = "Square is being held";}              //will be TRUE if button was JUST released
		
		if(1) { //print stick values if either is TRUE
			Serial.print("Stick Values:");
			Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX
			analog_LY = ps2x.Analog(PSS_LY);
			Serial.print(",");
			Serial.print(ps2x.Analog(PSS_LX), DEC);
			analog_LX = ps2x.Analog(PSS_LX);
			Serial.print(",");
			Serial.print(ps2x.Analog(PSS_RY), DEC);
			analog_RY = ps2x.Analog(PSS_RY);
			Serial.print(",");
			Serial.println(ps2x.Analog(PSS_RX), DEC);
			analog_RX = ps2x.Analog(PSS_RX);
		}
	}
}