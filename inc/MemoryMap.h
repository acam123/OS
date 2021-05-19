#pragma once
#include "Typedefs.h"
#include "TextPrint.h"

typedef struct memoryMapEntry {
	uint_64 BaseAddress;
	uint_64 RegionLength;
	uint_32 RegionType;
	uint_32 ExtendedAttributes;
} MemoryMapEntry;

extern uint_8 MemoryRegionCount;

void PrintMemoryMap(MemoryMapEntry* memMap);
void PrintMemoryMaps();
void PrintUsableMemoryMaps();
uint_32 GetUsableMemoryRegionCount();

