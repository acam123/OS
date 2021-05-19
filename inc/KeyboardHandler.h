#pragma once

#include "Typedefs.h"
#include "KeyboardScanCodeSet.h"
#include "TextPrint.h"


extern uint_8 LeftShiftOn;
extern uint_8 RightShiftOn;
extern uint_8 CapsLockOn;
extern uint_8 LastScanCode;

void StandardKeyboardHandler(uint_8 scanCode, uint_8 chr);
void KeyboardHandler0xe0(uint_8 scanCode);
void KeyboardHandler(uint_8 scanCode, uint_8 chr);