#include "KeyboardHandler.h"

uint_8 LeftShiftOn = 0;
uint_8 RightShiftOn = 0;
uint_8 CapsLockOn = 0;
uint_8 LastScanCode = 0x00;

void StandardKeyboardHandler(uint_8 scanCode, uint_8 chr) {
	if (chr != 0) { 
		if (!(LeftShiftOn|RightShiftOn|CapsLockOn) ) {
			PrintChar(chr);
		}
		else {
			PrintChar(ScanCodeLookupTableShift[scanCode]);
		}
	}
	else {
		switch (scanCode) {
			case 0x8e: // Backspace
				SetCursorPosition(CursorPosition - 1);
				PrintChar(chr);
				SetCursorPosition(CursorPosition - 1);
				break;
			case 0x2a: // Left Shift
				LeftShiftOn = 1;
				break;
			case 0xaa: // Left Shift
				LeftShiftOn = 0;
				break;
			case 0x36: // Left Shift
				RightShiftOn = 1;
				break;
			case 0xb6: // Left Shift
				RightShiftOn = 0;
				break;
			case 0x9c: // Return
				//PrintString("\n\r");
				PrintString("\n\r");
				//SetCursorPosition(CursorPosition + VGA_WIDTH - CursorPosition%VGA_WIDTH);
				break;
			case 0x0f: // Tab
				PrintString("    ");
				break;
			case 0x01: // Esc
				PrintString("[ESC]");
				break;
			case 0x38: // Alt
				PrintString("[ALT]");
				break;
			case 0x1d: // Ctrl
				PrintString("[CTRL]");
				break;
			case 0x3a: //Caps 
				CapsLockOn = (CapsLockOn == 1 ? 0 : 1);
				break;
			default:
				break;
		}
	}
}

void KeyboardHandler0xe0(uint_8 scanCode) {
	switch (scanCode) {
		case 0x50: // Down Arrow
			SetCursorPosition(CursorPosition + VGA_WIDTH);
			break;
		case 0x48: // Up Arrow
			SetCursorPosition(CursorPosition - VGA_WIDTH);
			break;
		case 0x4d: // Right Arrow
			SetCursorPosition(++CursorPosition);
			break;
		case 0x4b: // Left Arrow
			SetCursorPosition(--CursorPosition);
			break;
		case 0x1d: // Right Ctrl
			PrintString("[rCTRL]");
			break;
		case 0x38: // Right Alt
			PrintString("[rATL]");
			break;
		default:
			break;
	}

}

void KeyboardHandler(uint_8 scanCode, uint_8 chr) {
	switch (LastScanCode) {
		case 0xe0:
			KeyboardHandler0xe0(scanCode);
			break;
		default:
			StandardKeyboardHandler(scanCode, chr);
			break;
	}
	LastScanCode = scanCode;
}