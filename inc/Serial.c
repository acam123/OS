#include "Serial.h"

void init_serial() {

	out_8(COM1 + 1, 0x00);    // Disable all interrupts
	out_8(COM1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	out_8(COM1 + 0, 0x01);    // Set divisor to 1 (lo byte) 38400 baud
	out_8(COM1 + 1, 0x00);    //                  (hi byte)
 	out_8(COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
	out_8(COM1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   	out_8(COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
   	out_8(COM1 + 4, 0x1E);    // Set in loopback mode, test the serial chip
   	out_8(COM1 + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
 
   // Check if serial is faulty (i.e: not same byte as sent)
   if(in_8(COM1 + 0) != 0xAE) {
      return;
   }
 
   // If serial is not faulty set it in normal operation mode
   // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
   out_8(COM1 + 4, 0x0F);
   return;
	
}

uint64_t serial_received() {
   return in_8(COM1 + 5) & 1;
}
 
uint8_t read_serial() {
   while (serial_received() == 0);
 
   return in_8(COM1);
}

uint64_t is_transmit_empty() {
   return in_8(COM1 + 5) & 0x20;
}
 
void write_serial(uint8_t a) {
   while (is_transmit_empty() == 0);
 
   out_8(COM1,a);
}