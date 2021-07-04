#include "PageAllocator.h"

uint_64 freeMemory; // in bytes
uint_64 usedMemory; // in bytes
uint_64 reservedMemory; // in bytes
uint_64 totalMemory; // in bytes

void initAlloc() {
	initBitmap();
	// Free Memory should only be page aligned within BIOS entries type 1
	// ??? Is there important memory (like kernel) in first free memory entry? Should free memory only be second free memory section???
	
	totalMemory = (*BITMAP_SIZE) * PAGE_SIZE;
	freeMemory = 0;
	usedMemory = 0;
	reservedMemory = 0;
	initFreeMemory();
	ReserveSystemMemory();
	LockBitmap();
	return;
}

void freePage (uint_64 addr) {
	uint_64 bitIndx = (uint_64) addr / PAGE_SIZE;
	if ( BitmapGet(bitIndx) == 0) {
		return;
	}
	else { 
		BitmapSet(bitIndx, 0);
		freeMemory += PAGE_SIZE;
		usedMemory -= PAGE_SIZE;
		return;
	}
}

void freePages (uint_64 addr, uint_64 numPages) {
	for (uint_64 i=0; i<numPages; i++) {
		freePage(addr + i*PAGE_SIZE);
	} 
}

void lockPage (uint_64 addr) {
	uint_64 bitIndx = addr / PAGE_SIZE;
	if ( BitmapGet(bitIndx) == 1) {
		return;
	}
	else { 
		BitmapSet(bitIndx, 1);
		freeMemory -= PAGE_SIZE;
		usedMemory += PAGE_SIZE;
		return;
	}
}

void lockPages (uint_64 addr, uint_64 numPages) {
	for (uint_64 i=0; i<numPages; i++) {
		lockPage(addr + i*PAGE_SIZE);
	}
}

void unReservePage (uint_64 addr) {
	uint_64 bitIndx = addr / PAGE_SIZE;
	if ( BitmapGet(bitIndx) == 0) {
		return;
	}
	else { 
		BitmapSet(bitIndx, 0);
		reservedMemory -= PAGE_SIZE;
		return;
	}
}

void reservePage (uint_64 addr) {
	uint_64 bitIndx = (uint_64) addr / PAGE_SIZE;
	if ( BitmapGet(bitIndx) == 1) {
		return;
	}
	else { 
		BitmapSet(bitIndx, 1);
		reservedMemory += PAGE_SIZE;
		return;
	}
}

void reservePages (uint_64 addr, uint_64 numPages) {
	for (uint_64 i=0; i<numPages; i++) {
		reservePage( addr + i*PAGE_SIZE);
	}
}

void printMemoryAllocation() {
	PrintString("freeMemory: ");
	PrintString(IntToStr(freeMemory / 1024));
	PrintString(" KB\n\r");
	PrintString("usedMemory: ");
	PrintString(IntToStr(usedMemory / 1024));
	PrintString(" KB\n\r");
	PrintString("reservedMemory: ");
	PrintString(IntToStr(reservedMemory / 1024));
	PrintString(" KB\n\r");
	PrintString("totalMemory: ");
	PrintString(IntToStr(totalMemory / 1024));
	PrintString(" KB");
}

uint_64 pageAlign (uint_64 addr) {
	if (addr % PAGE_SIZE != 0) {
		addr = addr + PAGE_SIZE - addr % PAGE_SIZE;
	}
	return addr;
}

void LockBitmap() {
	uint_64 bytesOfBitmap = byteAlign(*BITMAP_SIZE)/8;
	uint_64 numPagesForBitmap = pageAlign(bytesOfBitmap)/PAGE_SIZE;
	lockPages((uint_64) ptrBitmap, numPagesForBitmap );
}

void initFreeMemory() {
	MemoryMapEntry* freeSection = GetLargestUsableMemoryEntry();
	uint_64 freeMemStartAddr = pageAlign(freeSection->BaseAddress);
	uint_64 numPages = freeSection->RegionLength / PAGE_SIZE;
	freePages(freeMemStartAddr, numPages);
	freeMemory = numPages * PAGE_SIZE;
}

void* requestPage() {
	for (uint_64 i=0; i < *BITMAP_SIZE; i++) {
		if (BitmapGet(i) == 0) {
			lockPage(i * PAGE_SIZE);
			return (void*) (i * PAGE_SIZE);
		}
	}
	return (void*) 0x0; //Page Frame Swap!!!
}
