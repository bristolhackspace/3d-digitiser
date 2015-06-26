/*
http://www.usdigital.com/support/user-guides/sei-absolute-encoder-communications-protocol

http://www.usdigital.com/support/user-guides/sei-serial-encoder-interface-bus
*/
#include <SoftwareSerial.h>

SoftwareSerial mySerial(8,9); // RX, TX
#define BUSY 7 //active low
#define DATA_DE 6 //active high
#define LED 13

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(DATA_DE, OUTPUT);
  pinMode(BUSY,INPUT);
  Serial.begin(9600);
Serial.println("started");
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
}

byte serial[4];
void loop() // run over and over
{
//	get_serial();
	//print_serial();
//	byte address = get_address();

	//set_address(2);
	read_pos(2);
	delay(100);
}

void print_serial()
{
	for(int i = 0; i < 4; i ++)
	{
	Serial.print(serial[i]);
	}
	Serial.println("");
}

void read_pos(byte address)
{
	Serial.print("enc ");
	Serial.print(address, HEX);
	Serial.print(" = ");
	 digitalWrite(DATA_DE,HIGH);
	delay(10);
 	mySerial.write(0x10 | address); //all devices, request position
	 digitalWrite(DATA_DE,LOW);
	 /*
	delay(10);
	if( ! digitalRead(BUSY))
	{
	Serial.println("no busy"); return;
	}
	*/

    unsigned int pos = mySerial.read() * 0xFF;
	pos += mySerial.read();
	Serial.println(pos);
}

/*
Get Address:
6 bytes: request byte, 06, 4 bytes serial number.
Returns 2 bytes: 1 byte serial and checksum if command is successful, only if serial number matches.
The encoder compares its serial number with the one supplied; if they match, it returns its serial (0 to E). Otherwise, it returns nothing.
*/
byte get_address()
{
	 digitalWrite(DATA_DE,HIGH);
	mySerial.write(0xFF);
	delay(10);
	if( ! digitalRead(BUSY))
	{
	Serial.println("no busy"); return 0xFF;
	}
	mySerial.write(0x06);
	//serial
	for(int i = 0; i < 4; i ++)
	{
	mySerial.write(serial[i]);
	}
	digitalWrite(DATA_DE,LOW);
	delay(10);
	byte address = mySerial.read();
	//skip checksum
	mySerial.read();
	Serial.println(address);
	return address;

}

/*
Read Serial Number:
2 bytes: request byte, 03
Returns 5 bytes: 4 bytes serial number and checksum if command is successful.
*/
void get_serial()
{
	 digitalWrite(DATA_DE,HIGH);
	mySerial.write(0xFF);
	delay(10);
	if( ! digitalRead(BUSY))
	{
	Serial.println("no busy"); return;
	}
	mySerial.write(0x03);
	digitalWrite(DATA_DE,LOW);
	delay(10);
	serial[0] = mySerial.read();
	serial[1] = mySerial.read();
	serial[2] = mySerial.read();
	serial[3] = mySerial.read();
	//checksum byte
	mySerial.read();

}

/*
Assign Address:
7 bytes: request byte, 07, 4 bytes serial number, 1 byte address.
Returns checksum if command is successful.
The encoder compares its serial number with the one supplied; if they match, it assigns itself the address supplied (must be between 0 and E). The new address is stored in EEPROM, therefore, it will be effective after resets and power downs
*/
void set_address(byte address)
{
	 digitalWrite(DATA_DE,HIGH);
	mySerial.write(0xFF);
	delay(10);
	if( ! digitalRead(BUSY))
	{
	Serial.println("no busy"); return;
	}
	mySerial.write(0x07);
	//serial
	for(int i = 0; i < 4; i ++)
	{
	mySerial.write(serial[i]);
	}
	mySerial.write(address);
	digitalWrite(DATA_DE,LOW);
	delay(10);
	//skip checksum
	mySerial.read();
}
