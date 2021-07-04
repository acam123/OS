#include "Bitmap.h"

uint_64* BITMAP_SIZE; // # of pages of memory 
uint_64* ptrBitmap = 0x0;


void initBitmap() {
	uint_64 memorySize = GetSystemMemorySize();
	*BITMAP_SIZE = (uint_64) (memorySize / PAGE_SIZE); //num pages of memory (PAD!!!)
	ptrBitmap = (uint_64*) pageAlign(GetLargestUsableMemoryEntry()->BaseAddress);
	uint_64 byteALignedBitmapSize = byteAlign((*BITMAP_SIZE)); //pad bitmap to a byte
	for (uint_64 i=0; i<(byteALignedBitmapSize/8); i++) {
		*(ptrBitmap+i) = 0b00000000;
	}
	return;
}

uint_64 byteAlign (uint_64 addr) {
	if (addr % 8 != 0) {
		addr + 8 - addr % 8;
	}
	return addr;
}

void printBitMapSize() {
	PrintString("Bitmap Size: ");
	PrintString(HexToStr((uint_64)*BITMAP_SIZE));
	return;
}


void fillBitmap() {
	for (int i=0; i<(byteAlign(*BITMAP_SIZE)/8); i++) {
		//Bitmap[i] = i;
		*(ptrBitmap + i)  = i%256;
	}
	return;
}

void printBitmapHead() {
	for (int i=0; i<8; i++) {
		uint_8 byte = *(ptrBitmap + i);//Bitmap[i];
		for (int j=0; j<8; j++) {
			uint_8 mask = 0b10000000;
			mask = mask >> j;
			
			uint_8 bit = byte & mask;
			bit = bit >> (8-1-j);
			PrintString(IntToStr(bit));
			PrintString(", ");
		}
		PrintString("\n\r");
	}
	return;
}

void printBitmapFor(uint_64 addr) {
	uint_8 len = 8;
	uint_64 strt = addr / PAGE_SIZE;
	uint_8 bit;
	
	for (int i=0; i<len; i++) {
		for (int j=0; j<8; j++) {
			bit = BitmapGet(strt + i*8 + j);
			if (bit == '*') {
				PrintChar(bit);
			}
			else {
				PrintString(IntToStr(bit));
			}
			PrintString(", ");
		}
		PrintString("\n\r");
	}

	return;
}

void printBitmapTail() {
	uint_8 byte;
	uint_64 byteIndx;
	uint_8 tailLen = 8;
	for (int i=0; i<tailLen; i++) {
		byteIndx = byteAlign(*BITMAP_SIZE)/8 - tailLen + i;
		byte = *( ptrBitmap + byteIndx );
		for (int j=0; j<8; j++) {
			uint_8 mask = 0b10000000;
			mask = mask >> j;
			
			uint_8 bit = byte & mask;
			bit = bit >> (8-1-j);

			// Hide bitmap padding if present
			if ( byteIndx * 8 + j >= *BITMAP_SIZE ) { bit = '*';}
			PrintString(IntToStr(bit));
			PrintString(", ");
		}
		PrintString("\n\r");
	}
	return;
}

uint_8 BitmapGet(uint_64 indx) {
	uint_8 bit;
	if (indx < 0 | indx >= (*BITMAP_SIZE) ) {
		bit = '*';
	}
	else {
		uint_8 shift = indx % 8;
		uint_8 byte = *(ptrBitmap + indx / 8);
		uint_8 mask = 0b10000000 >> (shift);
		bit = (byte & mask) >> (8-1-shift);
	}
	return bit; 
}

void BitmapSet(uint_64 bitIndx, uint_8 bit) {
	uint_8 shift = bitIndx % 8;
	uint_64 byteIndx = bitIndx / 8;
	uint_8 byte = *(ptrBitmap + byteIndx);
	uint_8 mask = 1 << (8-1-shift);
	switch (bit) {
		case 0:
			byte &= ~mask;
			break;
		case 1:
			byte |= mask;
			break;
		default: 
			return;
	}
	*(ptrBitmap + byteIndx) = byte;
	return;
}

