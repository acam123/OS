#pragma once

#include "Typedefs.h"
#include "TextPrint.h"
#include "MemoryMap.h"

//#define BITMAP_SIZE 64
#define PAGE_SIZE 4096

extern uint_64* ptrBitmap;
extern uint_64* BITMAP_SIZE;

void fillBitmap();
void printBitmapHead();
void printBitmapFor(uint_64 addr);
void printBitmapTail();
void BitmapSet(uint_64 indx, uint_8 bit);
uint_8 BitmapGet(uint_64 indx);
void printBitMapSize(); 
void initBitmap();
uint_64 byteAlign (uint_64 addr);