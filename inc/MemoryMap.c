#include "MemoryMap.h"


extern uint_64 MemoryMapStart;

//extern uint_8 UsableMemoryRegionCount;
//extern MemoryMapEntry* UsableMemoryRegions[10];


void PrintMemoryMap( MemoryMapEntry* memMap ) {
	PrintString("\n\rMemory Base: "); 
	PrintString(IntToStr(memMap->BaseAddress));
	PrintString("\n\rRegion Length: "); 
	PrintString(IntToStr(memMap->RegionLength));
	PrintString("\n\rMemory Type: "); 
	PrintString(IntToStr(memMap->RegionType));
	PrintString("\n\rMemory Attributes: ");
	PrintString(HexToStr(memMap->ExtendedAttributes, sizeof(memMap->ExtendedAttributes)));
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
