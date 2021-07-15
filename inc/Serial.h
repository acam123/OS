
#ifndef _SERIAL_
#define _SERIAL_

#include "Typedefs.h" 
#include "Textprint.h"
#include "IO.h"


#define COM1 0x3F8
#define COM2 0x2F8

#define BAUD 115200

void init_serial();
uint64_t serial_received();
uint8_t read_serial();
uint64_t is_transmit_empty();
void write_serial(uint8_t a);

#endif