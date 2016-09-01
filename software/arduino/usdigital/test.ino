/*
http://www.usdigital.com/support/user-guides/sei-absolute-encoder-communications-protocol

http://www.usdigital.com/support/user-guides/sei-serial-encoder-interface-bus

electrical timing:
http://cdn.usdigital.com/assets/general/SEI_Data_Sheet.pdf
*/
#include <SoftwareSerial.h>
#include "a2abs.h"

//stupidly wasted 2 other gpios by connecting them to serial rx&tx
#define GPIO1 0
#define GPIO2 1
#define LED 2
#define GPIO3 3
#define BUTTON 5

void setup()
{
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  Serial.println("started");
  setup_a2();
}

void loop() // run over and over
{
	//read all the numbers out
	digitalWrite(LED,HIGH);
	char buf[20];
	sprintf(buf, "%04d,%04d,%04d", get_pos(0), get_pos(1), get_pos(2));
	Serial.println(buf);
	digitalWrite(LED,LOW);
	delay(100);
}

