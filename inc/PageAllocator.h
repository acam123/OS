#pragma once
#include "Typedefs.h"
#include "MemoryMap.h"
#include "Bitmap.h"

void initAlloc();

void freePage (uint_64 addr);
void freePages (uint_64 addr, uint_64 numPages);

void lockPage (uint_64 addr);
void lockPages (uint_64 addr, uint_64 numPages);

void unReservePage (uint_64 addr);

void reservePage (uint_64 addr);
void reservePages (uint_64 addr, uint_64 numPages);

void printMemoryAllocation();

void LockBitmap(); 

uint_64 pageAlign(uint_64 addr);

void initFreeMemory() ;

void* requestPage();

