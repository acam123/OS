#ifndef _IDT_
#define _IDT_

#include "Typedefs.h"
#include "IO.h"
#include "TextPrint.h"
#include "KeyboardScanCodeSet.h"
#include "Mouse.h"
#include "PIT.h"

// Interrupt Descriptor Table
typedef struct idt64 {
	uint16_t offset_low;
	uint16_t selector;
	uint8_t ist;
	uint8_t types_attr;
	uint16_t offset_mid;
	uint32_t offset_high;
	uint32_t zero;
} idt64;

extern void(*main_keyboard_handler)(uint8_t scan_code, uint8_t chr);
void init_idt();

#endif