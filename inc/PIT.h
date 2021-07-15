#ifndef _PIT_ 
#define _PIT_ 

#include "Typedefs.h"
#include "TextPrint.h"
#include "IO.h"

#define MAX_PIT_DIVISOR 			65535 

#define PIT_CHANNEL_0_DATA_PORT		0x40 // read/write ~ IRQ0
#define PIT_CHANNEL_1_DATA_PORT 	0x41 // read/write ~ don't use
#define PIT_CHANNEL_2_DATA_PORT     0x42 // read/write ~ speaker
#define PIT_MODE_COMMAND			0x43 //register

#define PIT_CH_2_GATE 				0x61 // set bit 0, read bit 5



extern double time_since_boot;
extern uint64_t base_frequency;
extern uint16_t pit_divisor;

void sleepd (double secs);
void sleep (uint64_t millisecs);
void set_divisor (uint16_t divisor);
uint64_t get_frequency();
void set_frequency(uint64_t frequency);
void tick();

void init_pit();

#endif