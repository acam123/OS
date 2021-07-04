#include "kernel.h"

//extern int maxofthree(int, int, int);
//extern char Test[];

//extern uint_64 AidanMemoryRegionStart;

extern uint_64 _Kernel_Start;
extern uint_64 _Kernel_End;
extern uint_64 _text_sect;
extern uint_64 _data_sect;
extern uint_64 _rodata_sect;
extern uint_64 _bss_sect;


//extern uint_64 MemoryMapStart;
//extern uint_32 PML4;

void test_tmp(uint_64* a, uint_64* b) {
	*a = *b;
	*b=7;
}

void _start() {		
	/*
	PrintString(Test); // LoadBin File Macro
	*/
	
	InitializeScreen(BACKGROUND_DEFAULT | FOREGROUND_DEFAULT); 
	InitializeIDT(); 
	MainKeyboardHandler = KeyboardHandler;
	initAlloc();
	extendIdentityMap();
	InitHeap();

	uint_64* tmp;

	uint_64* b;
	*b = 2;

	test_tmp(tmp, b);

	PrintString(IntToStr(*tmp));


	
	
	


	

	return; // while(true)
}
