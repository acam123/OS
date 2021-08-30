#ifndef _IO_
#define _IO_

#include "Typedefs.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xa0
#define PIC2_DATA 0xa1

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

void out_8 (uint16_t port, uint8_t val);
uint8_t in_8 (uint16_t port);

void out_16 (uint16_t port, uint16_t val);
uint16_t in_16 (uint16_t port);


void out_32 (uint16_t port, uint32_t val);
uint32_t in_32 (uint16_t port); 

void io_wait();

void remap_pic();

#endif
