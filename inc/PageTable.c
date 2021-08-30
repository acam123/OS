#include "PageTable.h"

/* NOTE!!! I think virtual addresses all have to just use the bottom 48 bits so
 * maybe need to remove top 16 on first pml4 request??? each time??? 
 */
void calc_page_map_index (uint64_t virt_addr, page_map_index* page_index) {
    page_index->offset = virt_addr & 0xfff;
    virt_addr >>= 12;
    page_index->p_i = virt_addr & 0x1ff;
    virt_addr >>= 9;
    page_index->pt_i = virt_addr & 0x1ff;
    virt_addr >>= 9;
    page_index->pd_i = virt_addr & 0x1ff;
    virt_addr >>= 9;
    page_index->pdp_i = virt_addr & 0x1ff;
    //Note top 16 bits of virtual address not used 
}

void* map_memory_rec (uint64_t virt_addr, page_table* table) {
	uint64_t mask;
	uint16_t index;
	uint8_t pt_depth = 4;
	for (uint8_t i=0; i<pt_depth; i++) {
		mask = 0x0000ff8000000000 >> 9*i; // first 16 bits not used
		index = (virt_addr & mask) >> (12 + 9*(pt_depth-1-i)); 
		table = page_table_crawl(index, table);
	}
	void* page = (void*) table;
	return page;
}



void map_memory(uint64_t virt_addr, uint64_t phys_addr, page_table* pml_4) {
	page_map_index p_index;
	calc_page_map_index(virt_addr, &p_index);
		
	page_table* pdp = page_table_crawl(p_index.pdp_i, pml_4);

	page_table* pd = page_table_crawl(p_index.pd_i, pdp);

	page_table* pt = page_table_crawl(p_index.pt_i, pd);

	page_table_entry pte = (page_table_entry) pt->entries[p_index.p_i];

	pte.address = (uint64_t) phys_addr >> 12;
	pte.present = 0b1;
	pte.read_write = 0b1;
	pt->entries[p_index.p_i] = pte;
	return;
}

page_table* page_table_crawl (uint64_t index, page_table* table ) {

	page_table_entry pte = (page_table_entry) table->entries[index]; 
	page_table* next_page_table;
	if (pte.present == 0b1) {
		next_page_table = (page_table*) ((uint64_t) pte.address << 12);
	} 
	else {
		//Create
		next_page_table = (page_table*) request_page();
		for (int i=0; i<PAGE_SIZE; i++) {
			*((uint64_t*)((uint64_t)next_page_table+i)) = 0b00000000;
		}

		pte.address = (uint64_t)next_page_table >> 12;
		pte.present = 0b1;
		pte.read_write = 0b1;
		table->entries[index] = pte;
	}
	return next_page_table; 
}

void print_page_table_entry(page_table_entry pte) {
	print_string("\n\r");
	print_string("Present:");
	print_string(int_to_str((uint8_t) pte.present));
	print_string("\n\r");
	print_string("ReadWrite:");
	print_string(int_to_str((uint8_t) pte.read_write));
	print_string("\n\r");
	print_string("UserSuper:");
	print_string(int_to_str((uint8_t) pte.user_super));
	print_string("\n\r");
	print_string("WriteThrough:");
	print_string(int_to_str((uint8_t) pte.write_through));
	print_string("\n\r");
	print_string("CacheDisabled:");
	print_string(int_to_str((uint8_t) pte.cache_disabled));
	print_string("\n\r");
	print_string("Accessed:");
	print_string(int_to_str((uint8_t) pte.accessed));
	print_string("\n\r");
	print_string("ignore0:");
	print_string(int_to_str((uint8_t) pte.ignore0));
	print_string("\n\r");
	print_string("LargerPages:");
	print_string(int_to_str((uint8_t) pte.larger_pages));
	print_string("\n\r");
	print_string("ignore1:");
	print_string(int_to_str((uint8_t) pte.ignore1));
	print_string("\n\r");
	print_string("Available:");
	print_string(int_to_str((uint8_t) pte.available));
	print_string("\n\r");
	print_string("Address:");
	print_string(hex_to_str((uint64_t) pte.address));
	print_string("\n\r");
}

void extend_identity_map() {
	/* 
    * SimplePaging.asm identity mapped up to 0x200000;
    * pml4 = 0x1000 (can we grab from a global variable instead???)
    * Then map rest of address space to 512MB (0x20000000)
    * (maybe get max automatically instead???)
    */
    page_table* pml4 = (page_table*)0x1000;
    uint64_t new_page_addr = 0x200000; // End of 1st Page of pt made in assembly
    for (uint64_t i=0; i<0x1fe00; i++) {
    	map_memory(new_page_addr + i*PAGE_SIZE, new_page_addr + i*PAGE_SIZE, pml4);
    }
}
