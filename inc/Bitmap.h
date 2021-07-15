#ifndef _BITMAP_
#define _BITMAP_

#include "Typedefs.h"
#include "TextPrint.h"
#include "MemoryMap.h"

//#define BITMAP_SIZE 64
#define PAGE_SIZE 4096

extern uint64_t* ptr_bitmap;
extern uint64_t* BITMAP_SIZE;

void fill_bitmap ();
void print_bitmap_head ();
void print_bitmap_for (uint64_t addr);
void print_bitmap_tail ();
void bitmap_set (uint64_t indx, uint8_t bit);
uint8_t bitmap_get (uint64_t indx);
void print_bitmap_size (); 
void init_bitmap ();
uint64_t byte_align (uint64_t addr);

#endif