#include "Mouse.h"

void mouse_wait() {
	uint64_t timeout = 100000;
	while (timeout--) {
		if ( (in_8(0x64) & 0b10) == 0 ) {
			return;
		}
	}
}

void mouse_wait_input() {
	uint64_t timeout = 100000;
	while (timeout--) {
		if (in_8(0x64) & 0b1) {
			return;
		}
	}
}

void mouse_write(uint8_t val) {
	mouse_wait();
	out_8(0x64, 0x04);
	mouse_wait();
	out_8(0x60, val);
}

uint8_t mouse_read() {
	mouse_wait_input();
	return in_8(0x60);
}

void init_ps2_mouse () {
	out_8(0x64, 0xA8); // enable auxhiliary device - mouse
	mouse_wait();
	out_8(0x64, 0x20); //tell keyboard controller we want to send command to the mouse
	mouse_wait_input();
	uint8_t status = in_8(0x60);
	status |= 0b10;
	mouse_wait();
	out_8(0x64, 0x60);
	mouse_wait();
	out_8(0x60, status); //set correct bit in compaq status byte

	mouse_write(0xF6);

	mouse_read(); //acknowledge

	mouse_write(0xF4); //enable mouse

	mouse_read(); //acknowledge
}