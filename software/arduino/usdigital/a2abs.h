#ifndef a2abs_h
#define a2abs_h

#include "Arduino.h"

#define BUSY 7 //active low
#define DATA_DE 6 //active high

void setup_a2();
unsigned int get_pos(byte address);
byte get_address(byte * serial);
byte * get_serial();
void set_address(byte address, byte * serial);

#endif
