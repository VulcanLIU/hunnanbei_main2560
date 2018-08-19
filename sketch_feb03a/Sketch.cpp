/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */


//Beginning of Auto generated function prototypes by Atmel Studio
//End of Auto generated function prototypes by Atmel Studio

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0,OUTPUT);
  pinMode(A1,OUTPUT);
  digitalWrite(A0,LOW);
  digitalWrite(A1,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.write(0x55);
  delay(10);
  Serial.write(0xAA);
  delay(10);
  Serial.write(0x02);
  delay(10);
  Serial.write(map(analogRead(A2),0,1024,0,255));
  delay(10);
  Serial.write(map(analogRead(A3),0,1024,0,255));
  delay(100);
}
