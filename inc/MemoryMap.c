#include "MemoryMap.h"


extern uint64_t memory_map_start;

//extern uint8_t Usablememory_region_count;
//extern memory_map_entry* UsableMemoryRegions[10];


void print_memory_map( memory_map_entry* mem_map ) {
	print_string("\n\rMemory Base: "); 
	print_string(hex_to_str(mem_map->base_address));
	print_string("\n\rRegion Length: "); 
	print_string(hex_to_str(mem_map->region_length));
	print_string("\n\rMemory Type: "); 
	print_string(int_to_str(mem_map->region_type));
	print_string("\n\rMemory Attributes: ");
	print_string(hex_to_str(mem_map->extended_attributes));
	print_string("\n\r");
}

void print_memory_maps() {
	memory_map_entry* mem_map = (memory_map_entry*) memory_map_start; // Base of 1st memory map entry defined in DetectMemory.asm
	for (uint8_t i=0; i<memory_region_count; i++) {
		print_memory_map(mem_map + i);
	}
}

void print_usable_memory_maps() {
	memory_map_entry* mem_map_start = (memory_map_entry*) memory_map_start; // Base of 1st memory map entry defined in DetectMemory.asm
	for (uint8_t i=0; i<memory_region_count; i++) {
		memory_map_entry* mem_map = (memory_map_entry*)mem_map_start+i;
		if (mem_map->region_type == 1) {
			print_memory_map(mem_map);
		}
	}
}

uint32_t get_usable_memory_region_count() {
	uint32_t count = 0;
	memory_map_entry* mem_map_start = (memory_map_entry*) memory_map_start; // Base of 1st memory map entry defined in DetectMemory.asm
	for (uint8_t i=0; i<memory_region_count; i++) {
		memory_map_entry* mem_map = (memory_map_entry*)mem_map_start+i;
		if (mem_map->region_type == 1) {
			count++;
		}
	}
	return count;
}

memory_map_entry* get_largest_usable_memory_entry() {
	memory_map_entry* largestMap = 0x0;
	uint64_t tmp_map_size = 0;
	memory_map_entry* mem_map_start = (memory_map_entry*) memory_map_start;
	for (uint8_t i=0; i<memory_region_count; i++) {
		memory_map_entry* mem_map = (memory_map_entry*)mem_map_start+i;
		if ( (mem_map->region_type == 1) && (mem_map->region_length > tmp_map_size) ) {
			largestMap = mem_map;
			tmp_map_size = mem_map->region_length;
		}
	}
	return largestMap;
}

uint64_t get_system_memory_size() {
	uint64_t sz = 0;
	memory_map_entry* mem_map_start = (memory_map_entry*) memory_map_start;
	for (uint8_t i=0; i<memory_region_count; i++) {
		memory_map_entry* mem_map = (memory_map_entry*)mem_map_start+i;
		sz += mem_map->region_length;
	}
	//!!! NOTE: MemoryMap doesn't include 0xa0000-0xf0000 so manually including !!!
	sz += 0xf0000 - 0xa0000;
	return sz;
}

// for Page Alloc
void reserve_system_memory() {
	// Base of 1st memory map entry defined in DetectMemory.asm
	memory_map_entry* mem_map_start = (memory_map_entry*) memory_map_start; 
	for (uint8_t i=0; i<memory_region_count; i++) {
		memory_map_entry* mem_map = (memory_map_entry*)mem_map_start+i;

		if (mem_map->region_type != 1) {
			uint64_t buffered_addr = page_align(mem_map->base_address + 1) - PAGE_SIZE;
			uint64_t buffered_num_pages = page_align(mem_map->region_length) / PAGE_SIZE;
			reserve_pages( buffered_addr, buffered_num_pages);
		}
	}

	//!!! NOTE: MemoryMap doesn't include 0xa0000-0xf0000 so manually including !!!
	uint64_t base = 0xa0000;
	uint64_t length = 0xf0000 - 0xa0000;
	uint64_t buffered_addr = page_align(base + 1) - PAGE_SIZE;
	uint64_t buffered_num_pages = page_align(length) / PAGE_SIZE;
	reserve_pages( buffered_addr, buffered_num_pages);

	// Reserve low Free Memory Block for Kernel 
	// Maybe this should be locked instead and just from Kernel_Start to Kernel_End
	memory_map_entry* mem_map = (memory_map_entry*) memory_map_start; 
	buffered_addr = page_align(mem_map->base_address + 1) - PAGE_SIZE;
	buffered_num_pages = page_align(mem_map->region_length) / PAGE_SIZE;
	reserve_pages( buffered_addr, buffered_num_pages);

	return;
}
