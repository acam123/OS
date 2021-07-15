#include "PageAllocator.h"

uint64_t free_memory; // in bytes
uint64_t used_memory; // in bytes
uint64_t reserved_memory; // in bytes
uint64_t total_memory; // in bytes

void init_page_alloc() {
	init_bitmap();
	// Free Memory should only be page aligned within BIOS entries type 1
	// ??? Is there important memory (like kernel) in first free memory entry? Should free memory only be second free memory section???
	
	total_memory = (*BITMAP_SIZE) * PAGE_SIZE;
	free_memory = 0;
	used_memory = 0;
	reserved_memory = 0;
	init_free_memory();
	reserve_system_memory();
	lock_bitmap();
	return;
}

void free_page (uint64_t addr) {
	uint64_t bit_indx = (uint64_t) addr / PAGE_SIZE;
	if ( bitmap_get(bit_indx) == 0) {
		return;
	}
	else { 
		bitmap_set(bit_indx, 0);
		free_memory += PAGE_SIZE;
		used_memory -= PAGE_SIZE;
		return;
	}
}

void free_pages (uint64_t addr, uint64_t num_pages) {
	for (uint64_t i=0; i<num_pages; i++) {
		free_page(addr + i*PAGE_SIZE);
	} 
}

void lock_page (uint64_t addr) {
	uint64_t bit_indx = addr / PAGE_SIZE;
	if ( bitmap_get(bit_indx) == 1) {
		return;
	}
	else { 
		bitmap_set(bit_indx, 1);
		free_memory -= PAGE_SIZE;
		used_memory += PAGE_SIZE;
		return;
	}
}

void lock_pages (uint64_t addr, uint64_t num_pages) {
	for (uint64_t i=0; i<num_pages; i++) {
		lock_page(addr + i*PAGE_SIZE);
	}
}

void unreserve_page (uint64_t addr) {
	uint64_t bit_indx = addr / PAGE_SIZE;
	if ( bitmap_get(bit_indx) == 0) {
		return;
	}
	else { 
		bitmap_set(bit_indx, 0);
		reserved_memory -= PAGE_SIZE;
		return;
	}
}

void reserve_page (uint64_t addr) {
	uint64_t bit_indx = (uint64_t) addr / PAGE_SIZE;
	if ( bitmap_get(bit_indx) == 1) {
		return;
	}
	else { 
		bitmap_set(bit_indx, 1);
		reserved_memory += PAGE_SIZE;
		return;
	}
}

void reserve_pages (uint64_t addr, uint64_t num_pages) {
	for (uint64_t i=0; i<num_pages; i++) {
		reserve_page( addr + i*PAGE_SIZE);
	}
}

void print_memory_allocation() {
	print_string("free_memory: ");
	print_string(int_to_str(free_memory / 1024));
	print_string(" KB\n\r");
	print_string("used_memory: ");
	print_string(int_to_str(used_memory / 1024));
	print_string(" KB\n\r");
	print_string("reserved_memory: ");
	print_string(int_to_str(reserved_memory / 1024));
	print_string(" KB\n\r");
	print_string("total_memory: ");
	print_string(int_to_str(total_memory / 1024));
	print_string(" KB");
}

uint64_t page_align (uint64_t addr) {
	if (addr % PAGE_SIZE != 0) {
		addr = addr + PAGE_SIZE - addr % PAGE_SIZE;
	}
	return addr;
}

void lock_bitmap() {
	uint64_t bytes_of_bitmap = byte_align(*BITMAP_SIZE)/8;
	uint64_t num_pages_for_bitmap = page_align(bytes_of_bitmap)/PAGE_SIZE;
	lock_pages((uint64_t) ptr_bitmap, num_pages_for_bitmap );
}

void init_free_memory() {
	memory_map_entry* free_section = get_largest_usable_memory_entry();
	uint64_t free_mem_start_addr = page_align(free_section->base_address);
	uint64_t num_pages = free_section->region_length / PAGE_SIZE;
	free_pages(free_mem_start_addr, num_pages);
	free_memory = num_pages * PAGE_SIZE;
}

void* request_page() {
	for (uint64_t i=0; i < *BITMAP_SIZE; i++) {
		if (bitmap_get(i) == 0) {
			lock_page(i * PAGE_SIZE);
			return (void*) (i * PAGE_SIZE);
		}
	}
	return (void*) 0x0; //Page Frame Swap!!!
}
