#include "TextPrint.h"

uint_16 CursorPosition; // 0-VGA_AREA, position of cursor relative to start of Viewable Screen Window

uint_16 ScreenOffset; // 0-(SCREEN_BUF_SIZE - VGA_AREA), position of start of Viewable Screen Window relative to start of ScreenBuffer

//uint_16 BufferPosition; // 0-SCREEN_BUF_SIZE, position of cursor relative to start of ScreenBuffer

uint_16 ScreenBuffer[SCREEN_BUF_SIZE];


void fillScreenBuf() {
	uint_8 txt = 'a';
	uint_16 fill;
	for (uint_16 i=0; i<SCREEN_BUF_SIZE; i++) {
		// check for new line
		if ( (i != 0) && (i % (VGA_WIDTH) == 0) ) {
			//update letter loop
			if (txt == 'z') {
				txt = 'A';
			}
			else {
				txt++;
			}
		}

		fill = 0x1f00 | txt;
		ScreenBuffer[i] = fill;
	}

	memcpy((char*)ScreenBuffer, (char*) VGA_MEMORY, SCREEN_BUF_SIZE);

	//Initialize ScreenOffsettIntoBuffer
	ScreenOffset = 0;
}

//Always write to buffer (fix in keyboard handler)
//Buffer all writes 1 line at a time

void SetCursorPosition (sint_16 newPosition) {
	// NOTE: newPosition is requested new value for CursorPosition (relative to Viewable Screen Window)

	// Check if newPosition is outisde of curent Viewable Screen Window bounds
	if (newPosition >= VGA_AREA) {
		// Check if newPosition is outside of ScreenBuffer bounds
		if ( (ScreenOffset + newPosition) >= SCREEN_BUF_SIZE ) {
			// CAN'T WRITE BEYOND BUFFER!!! Need to feed single line at a time in order to shift buffer !!!
			// Forget oldest line(s) of Screen Buffer and shift down to fit new content 
			newPosition = newPosition % VGA_WIDTH + VGA_AREA - VGA_WIDTH;
			memcpy((char*)(ScreenBuffer+VGA_WIDTH), (char*)(ScreenBuffer), SCREEN_BUF_SIZE*2 - VGA_WIDTH*2);
			ClearBufLine(BACKGROUND_DEFAULT | FOREGROUND_DEFAULT, SCREEN_BUF_SIZE/VGA_WIDTH -1);
			

		}
		else {
			// Slide ScreenOffset and update VGA_MEMORY to reflect
			ScreenOffset += VGA_WIDTH;//(((newPosition-VGA_AREA)/VGA_WIDTH)+1) * VGA_WIDTH;
			newPosition = newPosition % VGA_WIDTH + VGA_AREA - VGA_WIDTH;

		}
	}
	else if (newPosition < 0) {
		// Check if newPosition is outside the ScreenBuffer bounds
		if (ScreenOffset + newPosition < 0) {
			// Bound the ScreenOffset and Update VGA Memory to reflect
			ScreenOffset = 0;
			newPosition = VGA_WIDTH + newPosition; //%VGA_WIDTH
			
		}
		else {
			// Slide the ScreenOffset and Update VGA Memory to reflect
			ScreenOffset += (((newPosition+1)/VGA_WIDTH)-1) * VGA_WIDTH; 
			newPosition = ((newPosition+1)%VGA_WIDTH) - 1 + VGA_WIDTH; //VGA_WIDTH + (newPosition+1) % VGA_WIDTH - 1;//-1 * newPosition % VGA_WIDTH; 
			
		}

	}
	memcpy ( (char*)(ScreenBuffer + ScreenOffset), VGA_MEMORY, VGA_AREA*2);


	// Send position to screen
	outb(REG_SCREEN_CTRL, 0x0f); // cursor offset low byte
	outb(REG_SCREEN_DATA, (uint_8)(newPosition & 0xff));
	outb(REG_SCREEN_CTRL, 0x0e); // cursor offset high byte
	outb(REG_SCREEN_DATA, (uint_8)((newPosition >> 8) & 0xff));

	//Update global position variable
	CursorPosition = (uint_16) newPosition;
}

void SetCursorPositionOLD (sint_16 position) {
	if (position >= VGA_AREA) {
		// save updated terminal to buffer 
		memcpy(VGA_MEMORY,(char*)(ScreenBuffer+ScreenOffset), VGA_AREA*2);
		
		if (ScreenOffset < (SCREEN_BUF_SIZE - VGA_AREA)) {
			// set terminal screen to shift within buffer
			ScreenOffset += VGA_WIDTH;
		}
		else {
			// grab updated terminal window, lose oldest line and add a new line to buffer
			memcpy((char*)(ScreenBuffer+VGA_WIDTH), (char*)(ScreenBuffer), SCREEN_BUF_SIZE*2 - VGA_WIDTH*2);
			ClearBufLine(BACKGROUND_DEFAULT | FOREGROUND_DEFAULT, SCREEN_BUF_SIZE/VGA_WIDTH -1);
		}
		// update terminal window based on shift/update of buffer 
		memcpy((char*)(ScreenBuffer+ScreenOffset), (char*)(VGA_MEMORY), VGA_AREA*2);
		position = position - VGA_WIDTH;
	}
	else if (position < 0 ) {
		// save updated terminal to buffer 
		memcpy(VGA_MEMORY,(char*)(ScreenBuffer+ScreenOffset), VGA_AREA*2);
		if (ScreenOffset >= VGA_WIDTH) {
			// set terminal screen to shift within buffer
			ScreenOffset -= VGA_WIDTH;
		}
		// grab updated terminal window (possibly shifted) 
		memcpy((char*)(ScreenBuffer + ScreenOffset), (char*)VGA_MEMORY, VGA_AREA*2);
		position = position + VGA_WIDTH;
	}

	// Send position to screen
	outb(REG_SCREEN_CTRL, 0x0f); // cursor offset low byte
	outb(REG_SCREEN_DATA, (uint_8)(position & 0xff));
	outb(REG_SCREEN_CTRL, 0x0e); // cursor offset high byte
	outb(REG_SCREEN_DATA, (uint_8)((position >> 8) & 0xff));

	//Update global position variable
	CursorPosition = (uint_16) position;
}

void ClearBufLine(uint_64 color, uint_16 line) {
	uint_64 reg = 0;
	for (int i=0; i<4; i++) {
		reg += color << (8 + 16*i);
	}
	// Fill the VGA_AREA 
	uint_64* ptr = (uint_64*) ScreenBuffer+(VGA_WIDTH*line/4);
	for (int i=0; i<VGA_WIDTH/4; i++) {
		*ptr = reg; 
		ptr++;
	}
}
void ClearLine (uint_64 color, uint_16 line) {
	uint_64 reg = 0;
	for (int i=0; i<4; i++) {
		reg += color << (8 + 16*i);
	}
	// Fill the VGA_AREA 
	uint_64* ptr = (uint_64*) VGA_MEMORY+VGA_WIDTH*line/4;
	for (int i=0; i<VGA_WIDTH/4; i++) {
		*ptr = reg; 
		ptr++;
	}
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

void InitializeScreen (uint_64 color) {
	uint_64 reg = 0;

	// Fill the register w/ 64bits of clear code   
	for (int i=0; i<4; i++) {
		reg += color << (8 + 16*i);
	}

	// Fill the ScreenBuffer
	uint_64* ptr = (uint_64*) ScreenBuffer;
	for (int i=0; i<SCREEN_BUF_SIZE/4; i++) {
		*ptr = reg; 
		ptr++;
	}

	ScreenOffset = 0;
	//BufferPosition = 0;
	SetCursorPosition(0);

	memcpy((char*)ScreenBuffer, (char*)VGA_MEMORY, VGA_AREA*2);
}

uint_16 PositionFromCoords(uint_8 x, uint_8 y) {
	return y*VGA_WIDTH + x;
}

uint_8 xCoordFromPosition(uint_16 position) {
	return position % VGA_WIDTH;
}

uint_8 yCoordFromPosition(uint_16 position) {
	return (position / VGA_WIDTH) - (position % VGA_WIDTH);
}


void PrintString(const char* str) {
	uint_8* charPtr = (uint_8*)str;
	uint_16 index = CursorPosition;
	uint_16 tmp;
	uint_16 prev;
	uint_8 newLine = 0;
	while (*charPtr != 0) {
		switch (*charPtr) {
			case 10:
				index += VGA_WIDTH;
				break;
			case 13:
				index -= (index % VGA_WIDTH);
				break;
			default:
				prev = *(ScreenBuffer + ScreenOffset + index) & 0xff00;
				tmp = (uint_16) *charPtr & 0x00ff;
				*(ScreenBuffer + ScreenOffset + index) = prev | tmp;
				index++;
				if (index % VGA_WIDTH == 0) { newLine = 1;}
				break;
		}
		charPtr++;

		if (newLine == 1) {
			SetCursorPosition(index);
			newLine = 0;
		}
	}	
	SetCursorPosition(index);
}

void PrintStringOLD (const char* str) {
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

void memset(uint_64 val, uint_64* dest, uint_64 num) {
	if (1) {
		uint_8* valByte = (uint_8*)&val;
		for(uint_8* destByte = (uint_8*)dest; destByte < (uint_8*)(uint_64)dest + num; destByte++) {
			*destByte = *valByte;
		}
		return;
	}
}

/* Copy bytes from one place to another. */
void memcpy(char* src, char* dst, int len) {
	for (int i=0; i<len; i++) {
		*(dst+i) = *(src+i);
	} 
}

/* Reverse memcpy*/
void memcpyRev (char* src, char* dst, int len) {
	for (int i=0; i<len; i++) {
		*(dst+len-1-i) = *(src+len-1-i);
	}
}

void PrintCharOLD (char chr) {
	uint_16 index = CursorPosition;
	*(VGA_MEMORY + index * 2) = chr;
	SetCursorPosition(++index);
}

void PrintChar (char chr) {
	uint_16 index = CursorPosition;
	uint_16 prev = *(ScreenBuffer + ScreenOffset + index) & 0xff00;
	uint_16 tmp = (uint_16) chr & 0x00ff;
	*(ScreenBuffer + ScreenOffset + index) = prev | tmp;
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





