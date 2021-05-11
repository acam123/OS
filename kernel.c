#include "inc/Typedefs.c"
#include "inc/TextPrint.c"
#include "inc/TextModeColorCodes.c"

extern const char Test[];

void _start() {	

	ClearScreen(BACKGROUND_DEFAULT | FOREGROUND_DEFAULT); 
	PrintString("\rHello World!!!\n\rTHIS IS A NEW LINE!!!");

	HexToStr(0x1a2b3c4d, sizeof(0x1a2b3c4d));
	PrintString(HexToStrBuf);
	PrintChar('Z');
	PrintString("\n\r");
	PrintString(IntToStr(581994));


	PrintString(Test);

	return;
}
