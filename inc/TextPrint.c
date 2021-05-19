#include "TextPrint.h"

uint_16 CursorPosition;

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
	SetCursorPosition(0);
}

uint_16 PositionFromCoords(uint_8 x, uint_8 y) {
	return y*VGA_WIDTH + x;
}

void PrintString (const char* str) {
	
	uint_8* charPtr = (uint_8*)str;
	uint_16 index = CursorPosition;
	while (*charPtr != 0) {
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
	uint_16 index = CursorPosition;
	*(VGA_MEMORY + index * 2) = chr;
	SetCursorPosition(++index);
}

uint_64 HexLen(uint_64 val) {
	uint_64 sz = 1;
	while(val/16 > 0) {
		sz++;
		val/=16;
	}
	return sz;
}

char HexToStrBuf[128];
char* HexToStr(uint_64 val) {
	*HexToStrBuf = 0;
	uint_8* ptr;
	uint_8 tmp;
	uint_64 sz = HexLen(val);
	sz = (sz%2==0) ? sz/2 : (sz+1)/2; //pad to byte width
	for (int i=0; i<sz; i++) {
		ptr = (uint_8*) &val + sz - i - 1;
		tmp = (*ptr & 0xf0) >> 4;
		HexToStrBuf[2+i*2] = tmp + (tmp > 9 ? 87 : 48);
		tmp = (*ptr & 0x0f);
		HexToStrBuf[2+i*2+1] = tmp + (tmp > 9 ? 87 : 48);
	}
	HexToStrBuf[2+sz*2] = '\0';
	HexToStrBuf[0] = '0';
	HexToStrBuf[1] = 'x';
	return HexToStrBuf;
}

char IntToStrBuf[128];
char* IntToStr(uint_64 val) {
	int sz = 1; 
    int tmp = val;
    while ((tmp/10) > 0) {
        sz++;
        tmp /= 10;
    }

	*IntToStrBuf = 0;
	for (int i=0; i<sz; i++) {
		uint_8 r = val % 10;
		IntToStrBuf[sz-i-1] = r + 48;
		val /= 10;
	}

	IntToStrBuf[sz] = '\0';
	return IntToStrBuf;
}

void PrintAny(void* ptr, int sz, int typ) {
	switch (typ) {
		case STR:
			PrintString((char*)*((char**)ptr));
			break;
		case CHR:
			PrintChar((char)*((char*)ptr));
			break;
		case DEC:
			PrintString(IntToStr((int)*((int*)ptr)));
			break;
		case HEX:
			PrintString(HexToStr( (int)(*((int*)ptr)) ));
			break;
		case FLT:
			PrintString("Float");
			break;
		default:
			break;
	}
}





