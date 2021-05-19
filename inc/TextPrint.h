#pragma once
#include "IO.h"
#include "Typedefs.h"
#include "TextModeColorCodes.h"

#define VGA_MEMORY (uint_8*) 0xb8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_AREA (VGA_WIDTH * VGA_HEIGHT)

extern uint_16 CursorPosition;

enum pType {STR, CHR, DEC, HEX, FLT};

void SetCursorPosition (uint_16 position);
void ClearScreen (uint_64 color);
uint_16 PositionFromCoords(uint_8 x, uint_8 y);
void PrintString (const char* str);
void PrintChar (char chr);
char* HexToStr(uint_64 val, uint_8 sz);
char* IntToStr(uint_64 val);
void PrintAny(void* ptr, int sz, int typ);