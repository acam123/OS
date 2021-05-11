#pragma once
#include "IO.c"
#include "Typedefs.c"
#include "TextModeColorCodes.c"
#define VGA_MEMORY (uint_8*) 0xb8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_AREA (VGA_WIDTH * VGA_HEIGHT)

uint_8 CursorPosition;

void SetCursorPosition (uint_16 position) {
	outb(0x3d4, 0x0f);
	outb(0x3d5, (uint_8)(position & 0xff));
	outb(0x3d4, 0x0e);
	outb(0x3d5, (uint_8)((position >> 8) & 0xff));

	CursorPosition = position;
}

void ClearScreen (uint_64 color) {
	uint_64 reg = 0;

	// Fill the register w/ 64bits of clear code   
	for (int i=0; i<4; i++) {
		reg += color << (8 + 16*i);
	}

	// Fill the VGA_AREA 
	uint_64* ptr = (uint_64*) VGA_MEMORY;
	for (int i=0; i<VGA_AREA/4; i++) {
		*ptr = reg; 
		ptr++;
	}
}

uint_16 PositionFromCoords(uint_8 x, uint_8 y) {
	return y*VGA_WIDTH + x;
}

void PrintString (const char* str) {
	uint_8* charPtr = (uint_8*)str;
	uint_16 index = CursorPosition;
	while (*charPtr !=0) {
		switch (*charPtr) {
			case 10:
				index += VGA_WIDTH;
				break;
			case 13:
				index -= (index % VGA_WIDTH);
				break;
			default:
				*(VGA_MEMORY + index * 2) = *charPtr;
				index++;
		}
		charPtr++;
	}
	SetCursorPosition(index);
}

void PrintChar (char chr) {
	*(VGA_MEMORY + CursorPosition * 2) = chr;
	SetCursorPosition(CursorPosition++);
}

char HexToStrBuf[128];
char* HexToStr(uint_32 val, uint_8 sz) {
	*HexToStrBuf = 0;
	uint_8* ptr;
	uint_8 tmp;
	for (int i=0; i<sz; i++) {
		ptr = (uint_8*) &val + sz - i - 1;
		tmp = (*ptr & 0xf0) >> 4;
		HexToStrBuf[i*2] = tmp + (tmp > 9 ? 87 : 48);
		tmp = (*ptr & 0x0f);
		HexToStrBuf[i*2+1] = tmp + (tmp > 9 ? 87 : 48);
	}
	return HexToStrBuf;
}

char IntToStrBuf[128];
char* IntToStr(uint_32 val) {
	int numDigits = 0; 
    int tmp = val;
    while (tmp > 0) {
        numDigits++;
        tmp /= 10;
    }

	*IntToStrBuf = 0;
	for (int i=0; i < numDigits; i++) {
		uint_8 r = val % 10;
		IntToStrBuf[numDigits - i - 1] = r + 48;
		val /= 10;
	}

	IntToStrBuf[numDigits] = '\0';
	return IntToStrBuf;
}





