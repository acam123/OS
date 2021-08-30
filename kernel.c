#include "kernel.h"

//extern int maxofthree(int, int, int);
//extern char Test[];

//extern uint64_t AidanMemoryRegionStart;

extern uint64_t _kernel_start;
extern uint64_t _kernel_end;
extern uint64_t _text_sect;
extern uint64_t _data_sect;
extern uint64_t _rodata_sect;
extern uint64_t _bss_sect;


//extern uint64_t memory_map_start;
//extern uint32_t pml4;

void _start() {		
	/*
	print_string(Test); // LoadBin File Macro
	*/
	
	init_screen(BACKGROUND_DEFAULT | FOREGROUND_DEFAULT); 
	init_idt(); 
	main_keyboard_handler = keyboard_handler;
	init_page_alloc();
	extend_identity_map();
	init_heap();
	init_pci();
	init_pit();
	init_serial();
	init_ahci();
	//init_usb();
	init_uhci(); 




	

	while(1) {
		asm("hlt"); // hlt when nothing to do
	}
}
