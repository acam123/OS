#ifndef _PAGE_TABLE_ 
#define _PAGE_TABLE_ 

#include "Typedefs.h"
#include "MemoryMap.h"
#include "Bitmap.h"
#include "PageAllocator.h"
#include "TextPrint.h"

// are these bitfields safe???
typedef struct page_table_entry {
	uint8_t present :1;
	uint8_t read_write :1;
	uint8_t user_super :1;
	uint8_t write_through :1;
	uint8_t cache_disabled :1;
	uint8_t accessed :1;
	uint8_t ignore0 :1;
	uint8_t larger_pages :1;
	uint8_t ignore1 :1;
	uint8_t available :3;
	// address is actually 40 bits
	uint64_t address :52;
} page_table_entry;

typedef struct page_table {
	page_table_entry entries[512];
}__attribute__((aligned(PAGE_SIZE))) page_table;

typedef struct page_map_index {
	uint64_t	pdp_i;
	uint64_t	pd_i;
	uint64_t	pt_i;
	uint64_t	p_i;
	uint16_t offset;
} page_map_index;

void calc_page_map_index (uint64_t virt_addr, page_map_index* page_index);
void* map_memory_rec (uint64_t virt_addr, page_table* table); // 
void map_memory (uint64_t virt_addr, uint64_t phys_addr, page_table* pml_4);
page_table* page_table_crawl (uint64_t index, page_table* table );
void print_page_table_entry(page_table_entry pte);
void extend_identity_map();

#endif
