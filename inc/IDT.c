#include "IDT.h"

// Interrupt Vector Table in linker 
extern idt64 _idt[256];

// isr_x "C" wrapper defined in IDT.asm
extern uint64_t isr0;
extern uint64_t isr1;
extern uint64_t isr12;
extern uint64_t isr14;

extern void load_idt();

void init_idt() {

	// PIT 
	
	_idt[0+32].zero = 0;
	_idt[0+32].offset_low = (uint16_t)(((uint64_t)&isr0 & 0x000000000000ffff));
	_idt[0+32].offset_mid = (uint16_t)(((uint64_t)&isr0 & 0x00000000ffff0000) >> 16);
	_idt[0+32].offset_high = (uint32_t)(((uint64_t)&isr0 & 0xffffffff00000000) >> 32);
	_idt[0+32].ist = 0;
	_idt[0+32].selector = 0x08; // code segment selector
	_idt[0+32].types_attr = 0x8e; // 32-bit interrupt gate
	

	// Keyboard 
	_idt[1+32].zero = 0;
	_idt[1+32].offset_low = (uint16_t)(((uint64_t)&isr1 & 0x000000000000ffff));
	_idt[1+32].offset_mid = (uint16_t)(((uint64_t)&isr1 & 0x00000000ffff0000) >> 16);
	_idt[1+32].offset_high = (uint32_t)(((uint64_t)&isr1 & 0xffffffff00000000) >> 32);
	_idt[1+32].ist = 0;
	_idt[1+32].selector = 0x08; // code segment selector
	_idt[1+32].types_attr = 0x8e; // 32-bit interrupt gate
		
	// PS/2 Mouse
	
	_idt[12+32].zero = 0;
	_idt[12+32].offset_low = (uint16_t)(((uint64_t)(&isr12) & 0x000000000000ffff));
	_idt[12+32].offset_mid = (uint16_t)(((uint64_t)(&isr12) & 0x00000000ffff0000) >> 16);
	_idt[12+32].offset_high = (uint32_t)(((uint64_t)(&isr12) & 0xffffffff00000000) >> 32);
	_idt[12+32].ist = 0;
	_idt[12+32].selector = 0x08; // code segment selector
	_idt[12+32].types_attr = 0x8e; // 32-bit interrupt gate
	

	// Page Fault
	
	_idt[14+32].zero = 0;
	_idt[14+32].offset_low = (uint16_t)(((uint64_t)(&isr14) & 0x000000000000ffff));
	_idt[14+32].offset_mid = (uint16_t)(((uint64_t)(&isr14) & 0x00000000ffff0000) >> 16);
	_idt[14+32].offset_high = (uint32_t)(((uint64_t)(&isr14) & 0xffffffff00000000) >> 32);
	_idt[14+32].ist = 0;
	_idt[14+32].selector = 0x08; // code segment selector
	_idt[14+32].types_attr = 0x8e; // 32-bit interrupt gate

	

	remap_pic();

	

	
	// Keyboard Mask
	//outb(0x21, 0xfd); 
	//outb(0xa1, 0xff);
	

	// Keyboard & Mouse Mask
	//outb(0x21, 0xf9); 
	//outb(0xa1, 0xef);

	//Keyboard & PIT Mask
	//outb(0x21, 0xfc); 
	//outb(0xa1, 0xff);


	//Keyboard, Mouse, & PIT Mask
	//outb(0x21, 0xf8); 
	//outb(0xa1, 0xef);
	out_8(0x21, 0b11111000);
	out_8(0xa1, 0b11101111);




	load_idt();

	init_ps2_mouse();

}


extern void isr0_handler() {
	tick();
	out_8(0x20, 0x20);
	out_8(0xa0, 0x20);
}

void(*main_keyboard_handler)(uint8_t scan_code, uint8_t chr);
extern void isr1_handler() {
	uint8_t scan_code = in_8(0x60);
	uint8_t chr = 0;

	if (scan_code < 0x3a) { chr = scan_code_lookup_table[scan_code]; }
	if (main_keyboard_handler != 0) { main_keyboard_handler(scan_code, chr); }

	//print_string(hex_to_str(inb(0x60), sizeof(inb(0x60))));

	out_8(0x20, 0x20);
	out_8(0xa0, 0x20);
}

extern void isr12_handler() {

	uint8_t mouse_data = in_8(0x60);

	print_string("...Mouse...");

	out_8(0xa0, 0x20);
	out_8(0x20, 0x20);
	
}

extern void isr14_handler() {

	print_string("...PAGE FAULT...");

	out_8(0x20, 0x20);
	out_8(0xa0, 0x20);

	while(1);
}