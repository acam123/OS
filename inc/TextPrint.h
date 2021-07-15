#ifndef _TEXT_PRINT_ 
#define _TEXT_PRINT_

#include "IO.h"
#include "Typedefs.h"
#include "TextModeColorCodes.h"

#define VGA_MEMORY (uint8_t*) 0xb8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_AREA (VGA_WIDTH * VGA_HEIGHT)
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5
#define SCREEN_BUF_SIZE VGA_AREA*2

extern uint16_t cursor_position;

enum p_type {STR, CHR, DEC, HEX, FLT};


void fill_screen_buf();

void set_cursor_position (signed short position);
void clear_buf_line(uint64_t color, uint16_t line);
void clear_line(uint64_t color, uint16_t line);
void init_screen (uint64_t color);
void clear_screen (uint64_t color);
uint16_t position_from_coords(uint8_t x, uint8_t y);
void print_string (const char* str);
void print_buffer (const char* str);
void memset(uint64_t val, uint64_t* dest, uint64_t num);
void memcpy(char* src, char* dst, int len);
void memcpy_rev  (char* src, char* dst, int len);
void print_char (char chr);
uint64_t hex_len(uint64_t val);
char* hex_to_str(uint64_t val);
char* int_to_str(uint64_t val);
char* float_to_str(double val, uint8_t percision);
void print_any(void* ptr, int sz, int typ);
#endif
