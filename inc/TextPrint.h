#pragma once
#include "IO.h"
#include "Typedefs.h"
#include "TextModeColorCodes.h"

#define VGA_MEMORY (uint_8*) 0xb8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_AREA (VGA_WIDTH * VGA_HEIGHT)
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5
#define SCREEN_BUF_SIZE VGA_AREA*2

extern uint_16 CursorPosition;

enum pType {STR, CHR, DEC, HEX, FLT};


void fillScreenBuf();

void SetCursorPosition (signed short position);
void ClearBufLine(uint_64 color, uint_16 line);
void ClearLine (uint_64 color, uint_16 line);
void InitializeScreen (uint_64 color);
void ClearScreen (uint_64 color);
uint_16 PositionFromCoords(uint_8 x, uint_8 y);
void PrintString (const char* str);
void memcpy(char* src, char* dst, int len);
void memcpyRev (char* src, char* dst, int len);
void PrintChar (char chr);
uint_64 HexLen(uint_64 val);
char* HexToStr(uint_64 val);
char* IntToStr(uint_64 val);
void PrintAny(void* ptr, int sz, int typ);
