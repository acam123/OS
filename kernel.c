#include "kernel.h"

//extern int maxofthree(int, int, int);
//extern char Test[];

//extern uint_64 AidanMemoryRegionStart;



void _start() {		
	/*
	PrintString(Test); // LoadBin File Macro
	*/
	
	ClearScreen(BACKGROUND_DEFAULT | FOREGROUND_DEFAULT); 
	InitializeIDT();
	SetCursorPosition(0);
	MainKeyboardHandler = KeyboardHandler;




	//PrintString(IntToStr(MemoryRegionCount));
	//uint_32 x = GetUsableMemoryRegionCount();
	//PrintString(IntToStr(x));

	//int x = GetUsableMemoryRegionCount(); 
	//PrintString(IntToStr(x));
	//PrintString(IntToStr(AidanMemoryRegionStart));

	//PrintString(IntToHex(AidanMemoryRegionStart));

	//PrintMemoryMap((MemoryMapEntry*)0x5000);
	//PrintUsableMemoryMaps();

	PrintString("Hello World!!! \n\rTHIS is a NEW LINE!!!\n\r");

	char* s = "AIDAN";
	//PrintAny(s, 0, STR);
	char** sPtr = &s;
	PrintAny(sPtr, 0, STR);
	
	char c = 'W';
	char* cPtr = &c;
	PrintAny(cPtr, 0, CHR);

	int i = 57;
	int* iPtr = &i;
	PrintAny(iPtr, 0, DEC);

	int h = 0x32;
	int* hPtr = &h;
	PrintAny(hPtr, 0, HEX);

	//PrintChar(*(x));

	//PrintString(x);

	
	
	PrintUsableMemoryMaps();
	


	return;
}
