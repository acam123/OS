#include "KeyboardHandler.h"

uint8_t left_shift_on = 0;
uint8_t right_shift_on = 0;
uint8_t caps_lock_on = 0;
uint8_t last_scan_code = 0x00;

void standard_keyboard_handler(uint8_t scan_code, uint8_t chr) {
	if (chr != 0) { 
		if (!(left_shift_on|right_shift_on|caps_lock_on) ) {
			print_char(chr);
		}
		else {
			print_char(scan_code_lookup_tableShift[scan_code]);
		}
	}
	else {
		switch (scan_code) {
			case 0x8e: // Backspace
				set_cursor_position(cursor_position - 1);
				print_char(chr);
				set_cursor_position(cursor_position - 1);
				break;
			case 0x2a: // Left Shift
				left_shift_on = 1;
				break;
			case 0xaa: // Left Shift
				left_shift_on = 0;
				break;
			case 0x36: // Left Shift
				right_shift_on = 1;
				break;
			case 0xb6: // Left Shift
				right_shift_on = 0;
				break;
			case 0x9c: // Return
				//print_string("\n\r");
				print_string("\n\r");
				//set_cursor_position(cursor_position + VGA_WIDTH - cursor_position%VGA_WIDTH);
				break;
			case 0x0f: // Tab
				print_string("    ");
				break;
			case 0x01: // Esc
				print_string("[ESC]");
				break;
			case 0x38: // Alt
				print_string("[ALT]");
				break;
			case 0x1d: // Ctrl
				print_string("[CTRL]");
				break;
			case 0x3a: //Caps 
				caps_lock_on = (caps_lock_on == 1 ? 0 : 1);
				break;
			default:
				break;
		}
	}
}

void keyboard_handler_0xe0(uint8_t scan_code) {
	switch (scan_code) {
		case 0x50: // Down Arrow
			set_cursor_position(cursor_position + VGA_WIDTH);
			break;
		case 0x48: // Up Arrow
			set_cursor_position(cursor_position - VGA_WIDTH);
			break;
		case 0x4d: // Right Arrow
			set_cursor_position(++cursor_position);
			break;
		case 0x4b: // Left Arrow
			set_cursor_position(--cursor_position);
			break;
		case 0x1d: // Right Ctrl
			print_string("[rCTRL]");
			break;
		case 0x38: // Right Alt
			print_string("[rATL]");
			break;
		default:
			break;
	}

}

void keyboard_handler(uint8_t scan_code, uint8_t chr) {
	switch (last_scan_code) {
		case 0xe0:
			keyboard_handler_0xe0(scan_code);
			break;
		default:
			standard_keyboard_handler(scan_code, chr);
			break;
	}
	last_scan_code = scan_code;
}