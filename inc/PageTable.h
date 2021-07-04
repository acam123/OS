#pragma once 

#include "Typedefs.h"
#include "MemoryMap.h"
#include "Bitmap.h"
#include "PageAllocator.h"
#include "TextPrint.h"

// are these bitfields safe???
typedef struct pageTableEntry {
	uint_8 Present :1;
	uint_8 ReadWrite :1;
	uint_8 UserSuper :1;
	uint_8 WriteThrough :1;
	uint_8 CacheDisabled :1;
	uint_8 Accessed :1;
	uint_8 ignore0 :1;
	uint_8 LargerPages :1;
	uint_8 ignore1 :1;
	uint_8 Available :3;
	// address is actually 40 bits
	uint_64 Address :52;
} PageTableEntry;

typedef struct pageTable {
	PageTableEntry entries[512];
}__attribute__((aligned(PAGE_SIZE))) PageTable;

typedef struct pageMapIndex {
	uint_64	PDP_i;
	uint_64	PD_i;
	uint_64	PT_i;
	uint_64	P_i;
	uint_16 Offset;
} PageMapIndex;

void CalcPageMapIndex (uint_64 virtAddr, PageMapIndex* PageIndex);
void* MapMemoryRec (uint_64 virtAddr, PageTable* Table); // 
void MapMemory (uint_64 virtAddr, uint_64 physAddr, PageTable* PML_4);
PageTable* PageTableCrawl (uint_64 index, PageTable* Table );
void printPageTableEntry(PageTableEntry PTE);
void extendIdentityMap();

