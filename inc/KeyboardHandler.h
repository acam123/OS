#ifndef _KEYBOARD_HANDLER_
#define _KEYBOARD_HANDLER_

#include "Typedefs.h"
#include "KeyboardScanCodeSet.h"
#include "TextPrint.h"


extern uint8_t left_shift_on;
extern uint8_t right_shift_on;
extern uint8_t caps_lock_on;
extern uint8_t last_scan_code;

void standard_keyboard_handler(uint8_t scan_code, uint8_t chr);
void keyboard_handler_0xe0(uint8_t scan_code);
void keyboard_handler(uint8_t scan_code, uint8_t chr);

#endif