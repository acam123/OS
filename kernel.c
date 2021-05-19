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

	PrintString("Hello World!!! \n\rTHIS is a NEW LINE!!!\n\r");
	
	
	//PrintUsableMemoryMaps();
	

	return;
}
