/*
http://www.usdigital.com/support/user-guides/sei-absolute-encoder-communications-protocol

http://www.usdigital.com/support/user-guides/sei-serial-encoder-interface-bus

electrical timing:
http://cdn.usdigital.com/assets/general/SEI_Data_Sheet.pdf
*/
#include <SoftwareSerial.h>
#include "a2abs.h"
SoftwareSerial mySerial(8,9); // RX, TX

void setup_a2()
{
  pinMode(DATA_DE, OUTPUT);
  pinMode(BUSY,INPUT);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
}

	/*
	// set an address
	byte * serial = get_serial();
	Serial.print("address=");
	Serial.println(get_address(serial));
	set_address(0, serial);
	delay(100);
	return;
	*/

//single byte command
unsigned int get_pos(byte address)
{
	digitalWrite(DATA_DE,HIGH);
	//read position, with no status bytes
 	mySerial.write(0x01 << 4 | address);
	digitalWrite(DATA_DE,LOW);
	delay(1);
	if( ! digitalRead(BUSY))
	{
		Serial.println("no busy"); return -1;
	}

	//2 bytes, significant byte first
	while(mySerial.available()!=2);
	unsigned int pos = mySerial.read() << 8 | mySerial.read();

	//wait for encoder to stop talking
	while(digitalRead(BUSY));

	return pos;
}

/*
Get Address:
6 bytes: request byte, 06, 4 bytes serial number.
Returns 2 bytes: 1 byte serial and checksum if command is successful, only if serial number matches.
The encoder compares its serial number with the one supplied; if they match, it returns its serial (0 to E). Otherwise, it returns nothing.
*/
byte get_address(byte * serial)
{
	digitalWrite(DATA_DE,HIGH);
	mySerial.write(0xFF);
	delay(10);
	if( ! digitalRead(BUSY))
	{
		Serial.println("no busy"); 
		return -1;
	}

	mySerial.write(0x06);
	//serial
	for(int i = 0; i < 4; i ++)
	{
		mySerial.write(serial[i]);
	}
	digitalWrite(DATA_DE,LOW);
	//wait for device to release busy
	while(digitalRead(BUSY));

	byte address = mySerial.read();
	//skip checksum
	mySerial.read();
	//Serial.println(address);
	return address;

}

/*
Read Serial Number:
2 bytes: request byte, 03
Returns 5 bytes: 4 bytes serial number and checksum if command is successful.
*/
byte * get_serial()
{
	static byte serial[4];
	mySerial.flush();

	digitalWrite(DATA_DE,HIGH);
	mySerial.write(0xFF);
	delay(1);
	if( ! digitalRead(BUSY))
	{
	Serial.println("no busy"); return 0;
	}

	mySerial.write(0x03);
	digitalWrite(DATA_DE,LOW);
	//wait for device to release busy
	while(digitalRead(BUSY));
	serial[0] = mySerial.read();
	serial[1] = mySerial.read();
	serial[2] = mySerial.read();
	serial[3] = mySerial.read();
	//checksum byte
	mySerial.read();

	Serial.print("serial=");
	for(int i = 0; i < 4; i ++)
	{
		Serial.print(serial[i]);
		Serial.print(",");
	}
	Serial.println("");
	return serial;

}

/*
Assign Address:
7 bytes: request byte, 07, 4 bytes serial number, 1 byte address.
Returns checksum if command is successful.
The encoder compares its serial number with the one supplied; if they match, it assigns itself the address supplied (must be between 0 and E). The new address is stored in EEPROM, therefore, it will be effective after resets and power downs
*/
void set_address(byte address, byte * serial)
{
	digitalWrite(DATA_DE,HIGH);
	mySerial.write(0xFF);
	delay(1);
	if( ! digitalRead(BUSY))
	{
		Serial.println("no busy");
		return;
	}
	mySerial.write(0x07);
	//serial
	for(int i = 0; i < 4; i ++)
	{
		mySerial.write(serial[i]);
	}
	mySerial.write(address);
	digitalWrite(DATA_DE,LOW);
	//wait for device to release busy
	while(digitalRead(BUSY));
	//skip checksum
	mySerial.read();
}

