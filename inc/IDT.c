#include "IDT.h"

// Interrupt Vector Table in linker 
extern IDT64 _idt[256];

// Isr1 command from IDT.asm
extern uint_64 isr1;

extern void LoadIDT();

void InitializeIDT() {

	// IDT for Keyboard
	_idt[1].zero = 0;
	_idt[1].offset_low = (uint_16)(((uint_64)&isr1 & 0x000000000000ffff));
	_idt[1].offset_mid = (uint_16)(((uint_64)&isr1 & 0x00000000ffff0000) >> 16);
	_idt[1].offset_high = (uint_32)(((uint_64)&isr1 & 0xffffffff00000000) >> 32);
	_idt[1].ist = 0;
	_idt[1].selector = 0x08; // code segment selector
	_idt[1].types_attr = 0x8e; // 32-bit interrupt gate

	RemapPIC();

	outb(0x21, 0xfd);
	outb(0xa1, 0xff);

	LoadIDT();

}

void(*MainKeyboardHandler)(uint_8 scanCode, uint_8 chr);
extern void isr1_handler() {
	uint_8 scanCode = inb(0x60);
	uint_8 chr = 0;

	if (scanCode < 0x3a) { chr = ScanCodeLookupTable[scanCode]; }
	if (MainKeyboardHandler != 0) { MainKeyboardHandler(scanCode, chr); }

	//PrintString(HexToStr(inb(0x60), sizeof(inb(0x60))));
	outb(0x20, 0x20);
	outb(0xa0, 0x20);
}