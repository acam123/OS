#pragma once
#include "Typedefs.h"
#include "TextPrint.h"
#include "PageAllocator.h"

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
MemoryMapEntry* GetLargestUsableMemoryEntry();
uint_64 GetSystemMemorySize();
void ReserveSystemMemory();


