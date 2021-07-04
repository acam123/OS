#include "MemoryMap.h"


extern uint_64 MemoryMapStart;

//extern uint_8 UsableMemoryRegionCount;
//extern MemoryMapEntry* UsableMemoryRegions[10];


void PrintMemoryMap( MemoryMapEntry* memMap ) {
	PrintString("\n\rMemory Base: "); 
	PrintString(HexToStr(memMap->BaseAddress));
	PrintString("\n\rRegion Length: "); 
	PrintString(HexToStr(memMap->RegionLength));
	PrintString("\n\rMemory Type: "); 
	PrintString(IntToStr(memMap->RegionType));
	PrintString("\n\rMemory Attributes: ");
	PrintString(HexToStr(memMap->ExtendedAttributes));
	PrintString("\n\r");
}

void PrintMemoryMaps() {
	MemoryMapEntry* memMap = (MemoryMapEntry*) MemoryMapStart; // Base of 1st memory map entry defined in DetectMemory.asm
	for (uint_8 i=0; i<MemoryRegionCount; i++) {
		PrintMemoryMap(memMap + i);
	}
}

void PrintUsableMemoryMaps() {
	MemoryMapEntry* memMapStart = (MemoryMapEntry*) MemoryMapStart; // Base of 1st memory map entry defined in DetectMemory.asm
	for (uint_8 i=0; i<MemoryRegionCount; i++) {
		MemoryMapEntry* memMap = (MemoryMapEntry*)memMapStart+i;
		if (memMap->RegionType == 1) {
			PrintMemoryMap(memMap);
		}
	}
}

uint_32 GetUsableMemoryRegionCount() {
	uint_32 count = 0;
	MemoryMapEntry* memMapStart = (MemoryMapEntry*) MemoryMapStart; // Base of 1st memory map entry defined in DetectMemory.asm
	for (uint_8 i=0; i<MemoryRegionCount; i++) {
		MemoryMapEntry* memMap = (MemoryMapEntry*)memMapStart+i;
		if (memMap->RegionType == 1) {
			count++;
		}
	}
	return count;
}

MemoryMapEntry* GetLargestUsableMemoryEntry() {
	MemoryMapEntry* largestMap = 0x0;
	uint_64 tmpMapSize = 0;
	MemoryMapEntry* memMapStart = (MemoryMapEntry*) MemoryMapStart;
	for (uint_8 i=0; i<MemoryRegionCount; i++) {
		MemoryMapEntry* memMap = (MemoryMapEntry*)memMapStart+i;
		if ( (memMap->RegionType == 1) && (memMap->RegionLength > tmpMapSize) ) {
			largestMap = memMap;
			tmpMapSize = memMap->RegionLength;
		}
	}
	return largestMap;
}

uint_64 GetSystemMemorySize() {
	uint_64 sz = 0;
	MemoryMapEntry* memMapStart = (MemoryMapEntry*) MemoryMapStart;
	for (uint_8 i=0; i<MemoryRegionCount; i++) {
		MemoryMapEntry* memMap = (MemoryMapEntry*)memMapStart+i;
		sz += memMap->RegionLength;
	}
	//!!! NOTE: MemoryMap doesn't include 0xa0000-0xf0000 so manually including !!!
	sz += 0xf0000 - 0xa0000;
	return sz;
}

// for Page Alloc
void ReserveSystemMemory() {
	// Base of 1st memory map entry defined in DetectMemory.asm
	MemoryMapEntry* memMapStart = (MemoryMapEntry*) MemoryMapStart; 
	for (uint_8 i=0; i<MemoryRegionCount; i++) {
		MemoryMapEntry* memMap = (MemoryMapEntry*)memMapStart+i;

		if (memMap->RegionType != 1) {
			uint_64 bufferedAddr = pageAlign(memMap->BaseAddress + 1) - PAGE_SIZE;
			uint_64 bufferedNumPages = pageAlign(memMap->RegionLength) / PAGE_SIZE;
			reservePages( bufferedAddr, bufferedNumPages);
		}
	}

	//!!! NOTE: MemoryMap doesn't include 0xa0000-0xf0000 so manually including !!!
	uint_64 base = 0xa0000;
	uint_64 length = 0xf0000 - 0xa0000;
	uint_64 bufferedAddr = pageAlign(base + 1) - PAGE_SIZE;
	uint_64 bufferedNumPages = pageAlign(length) / PAGE_SIZE;
	reservePages( bufferedAddr, bufferedNumPages);

	// Reserve low Free Memory Block for Kernel 
	// Maybe this should be locked instead and just from Kernel_Start to Kernel_End
	MemoryMapEntry* memMap = (MemoryMapEntry*) MemoryMapStart; 
	bufferedAddr = pageAlign(memMap->BaseAddress + 1) - PAGE_SIZE;
	bufferedNumPages = pageAlign(memMap->RegionLength) / PAGE_SIZE;
	reservePages( bufferedAddr, bufferedNumPages);

	return;
}
