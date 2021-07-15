#include "Bitmap.h"

uint64_t* BITMAP_SIZE; // # of pages of memory 
uint64_t* ptr_bitmap = 0x0;


void init_bitmap() {
	uint64_t memory_size = get_system_memory_size();
	*BITMAP_SIZE = (uint64_t) (memory_size / PAGE_SIZE); //num pages of memory (PAD!!!)
	ptr_bitmap = (uint64_t*) page_align(get_largest_usable_memory_entry()->base_address);
	uint64_t byte_aligned_bitmap_size = byte_align((*BITMAP_SIZE)); //pad bitmap to a byte
	for (uint64_t i=0; i<(byte_aligned_bitmap_size/8); i++) {
		*(ptr_bitmap+i) = 0b00000000;
	}
	return;
}

uint64_t byte_align (uint64_t addr) {
	if (addr % 8 != 0) {
		addr + 8 - addr % 8;
	}
	return addr;
}

void print_bitmap_size() {
	print_string("Bitmap Size: ");
	print_string(hex_to_str((uint64_t)*BITMAP_SIZE));
	return;
}


void fill_bitmap() {
	for (int i=0; i<(byte_align(*BITMAP_SIZE)/8); i++) {
		//Bitmap[i] = i;
		*(ptr_bitmap + i)  = i%256;
	}
	return;
}

void print_bitmap_head() {
	for (int i=0; i<8; i++) {
		uint8_t byte = *(ptr_bitmap + i);//Bitmap[i];
		for (int j=0; j<8; j++) {
			uint8_t mask = 0b10000000;
			mask = mask >> j;
			
			uint8_t bit = byte & mask;
			bit = bit >> (8-1-j);
			print_string(int_to_str(bit));
			print_string(", ");
		}
		print_string("\n\r");
	}
	return;
}

void print_bitmap_for(uint64_t addr) {
	uint8_t len = 8;
	uint64_t strt = addr / PAGE_SIZE;
	uint8_t bit;
	
	for (int i=0; i<len; i++) {
		for (int j=0; j<8; j++) {
			bit = bitmap_get(strt + i*8 + j);
			if (bit == '*') {
				print_char(bit);
			}
			else {
				print_string(int_to_str(bit));
			}
			print_string(", ");
		}
		print_string("\n\r");
	}

	return;
}

void print_bitmap_tail() {
	uint8_t byte;
	uint64_t byte_indx;
	uint8_t tail_len = 8;
	for (int i=0; i<tail_len; i++) {
		byte_indx = byte_align(*BITMAP_SIZE)/8 - tail_len + i;
		byte = *( ptr_bitmap + byte_indx );
		for (int j=0; j<8; j++) {
			uint8_t mask = 0b10000000;
			mask = mask >> j;
			
			uint8_t bit = byte & mask;
			bit = bit >> (8-1-j);

			// Hide bitmap padding if present
			if ( byte_indx * 8 + j >= *BITMAP_SIZE ) { bit = '*';}
			print_string(int_to_str(bit));
			print_string(", ");
		}
		print_string("\n\r");
	}
	return;
}

uint8_t bitmap_get(uint64_t indx) {
	uint8_t bit;
	if (indx < 0 | indx >= (*BITMAP_SIZE) ) {
		bit = '*';
	}
	else {
		uint8_t shift = indx % 8;
		uint8_t byte = *(ptr_bitmap + indx / 8);
		uint8_t mask = 0b10000000 >> (shift);
		bit = (byte & mask) >> (8-1-shift);
	}
	return bit; 
}

void bitmap_set(uint64_t bit_indx, uint8_t bit) {
	uint8_t shift = bit_indx % 8;
	uint64_t byte_indx = bit_indx / 8;
	uint8_t byte = *(ptr_bitmap + byte_indx);
	uint8_t mask = 1 << (8-1-shift);
	switch (bit) {
		case 0:
			byte &= ~mask;
			break;
		case 1:
			byte |= mask;
			break;
		default: 
			return;
	}
	*(ptr_bitmap + byte_indx) = byte;
	return;
}

