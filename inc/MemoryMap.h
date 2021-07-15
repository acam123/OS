#ifndef _MEMORY_MAP_ 
#define _MEMORY_MAP_ 

#include "Typedefs.h"
#include "TextPrint.h"
#include "PageAllocator.h"

typedef struct memoryMapEntry {
	uint64_t base_address;
	uint64_t region_length;
	uint32_t region_type;
	uint32_t extended_attributes;
} memory_map_entry;

extern uint8_t memory_region_count;

void print_memory_map(memory_map_entry* mem_map);
void print_memory_maps();
void print_usable_memory_maps();
uint32_t get_usable_memory_region_count();
memory_map_entry* get_largest_usable_memory_entry();
uint64_t get_system_memory_size();
void reserve_system_memory();

#endif
