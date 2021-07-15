#ifndef _PAGE_ALLOCATOR_ 
#define _PAGE_ALLOCATOR_ 


#include "Typedefs.h"
#include "MemoryMap.h"
#include "Bitmap.h"

void init_page_alloc();

void free_page (uint64_t addr);
void free_pages (uint64_t addr, uint64_t num_pages);

void lock_page (uint64_t addr);
void lock_pages (uint64_t addr, uint64_t num_pages);

void unreserve_page (uint64_t addr);

void reserve_page (uint64_t addr);
void reserve_pages (uint64_t addr, uint64_t num_pages);

void print_memory_allocation();

void lock_bitmap(); 

uint64_t page_align(uint64_t addr);

void init_free_memory() ;

void* request_page();

#endif