#include "PIT.h"

double time_since_boot = 0;
uint64_t base_frequency = 1193182;
uint16_t pit_divisor = MAX_PIT_DIVISOR; 

void init_pit() {
	set_divisor(pit_divisor);
}

void set_divisor(uint16_t pit_divisor) {
	if (pit_divisor < 100) {
		pit_divisor = 100;
	}
	outb( PIT_CHANNEL_0_DATA_PORT, (uint8_t)(pit_divisor & 0x00ff) );
	io_wait();
	outb( PIT_CHANNEL_0_DATA_PORT, (uint8_t)((pit_divisor & 0xff00) >> 8) );
}

uint64_t get_frequency() {
	return base_frequency / pit_divisor;
}

void set_frequency (uint64_t frequency) {
	set_divisor (base_frequency / frequency);
}

void tick() {
	time_since_boot += (1 / (double)get_frequency());
}

void sleepd( double secs) {
	double start_time = time_since_boot;
	while (time_since_boot < start_time + secs) {
		asm("hlt");
	}
}

void sleep(uint64_t millisecs) {
	sleepd((double)millisecs / 1000);
}