#include "IO.h"

// 1 bytes
void outb (uint16_t port, uint8_t val) {
	asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

uint8_t inb (uint16_t port) {
	uint8_t return_val;
	asm volatile ("inb %1, %0" : "=a"(return_val) : "Nd"(port));
	return return_val;
}


// 4 bytes
void outl (uint16_t port, uint32_t val) {
	asm volatile ("outl %%eax, %%dx" : : "a"(val), "Nd"(port));
}

uint32_t inl (uint16_t port) {
	uint32_t return_val;
	asm volatile ("inl %%dx, %%eax" : "=a"(return_val) : "Nd"(port));
	return return_val;
}

void io_wait() {
	asm volatile ("outb %%al, $0x80" : : "a"(0));
}



void remap_pic() {
	uint8_t a1, a2;
	a1 = inb(PIC1_DATA);
	a2 = inb(PIC2_DATA);

	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4); 
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	outb(PIC1_DATA, 0x20); // 0x20??? 0
	outb(PIC2_DATA, 0x28); // 0x28??? 8

	outb(PIC1_DATA, 4);
	outb(PIC2_DATA, 2);
	outb(PIC1_DATA, ICW4_8086);
	outb(PIC2_DATA, ICW4_8086);

	outb(PIC1_DATA, a1);
	outb(PIC2_DATA, a2);
}

/*
#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01
*/