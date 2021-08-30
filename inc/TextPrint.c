#include "TextPrint.h"

uint16_t cursor_position; // 0-VGA_AREA, position of cursor relative to start of Viewable Screen Window

uint16_t screen_offset; // 0-(SCREEN_BUF_SIZE - VGA_AREA), position of start of Viewable Screen Window relative to start of screen_buffer

//uint16_t BufferPosition; // 0-SCREEN_BUF_SIZE, position of cursor relative to start of screen_buffer

uint16_t screen_buffer[SCREEN_BUF_SIZE];


void fill_screen_buf() {
	uint8_t txt = 'a';
	uint16_t fill;
	for (uint16_t i=0; i<SCREEN_BUF_SIZE; i++) {
		// check for new line
		if ( (i != 0) && (i % (VGA_WIDTH) == 0) ) {
			//update letter loop
			if (txt == 'z') {
				txt = 'A';
			}
			else {
				txt++;
			}
		}

		fill = 0x1f00 | txt;
		screen_buffer[i] = fill;
	}

	memcpy((char*)screen_buffer, (char*) VGA_MEMORY, SCREEN_BUF_SIZE);

	//Initialize screen_offsettIntoBuffer
	screen_offset = 0;
}

//Always write to buffer (fix in keyboard handler)
//Buffer all writes 1 line at a time

void set_cursor_position (sint16_t new_position) {
	// NOTE: new_position is requested new value for cursor_position (relative to Viewable Screen Window)

	// Check if new_position is outisde of curent Viewable Screen Window bounds
	if (new_position >= VGA_AREA) {
		// Check if new_position is outside of screen_buffer bounds
		if ( (screen_offset + new_position) >= SCREEN_BUF_SIZE ) {
			// CAN'T WRITE BEYOND BUFFER!!! Need to feed single line at a time in order to shift buffer !!!
			// Forget oldest line(s) of Screen Buffer and shift down to fit new content 
			new_position = new_position % VGA_WIDTH + VGA_AREA - VGA_WIDTH;
			memcpy((char*)(screen_buffer+VGA_WIDTH), (char*)(screen_buffer), SCREEN_BUF_SIZE*2 - VGA_WIDTH*2);
			clear_buf_line(BACKGROUND_DEFAULT | FOREGROUND_DEFAULT, SCREEN_BUF_SIZE/VGA_WIDTH -1);
			

		}
		else {
			// Slide screen_offset and update VGA_MEMORY to reflect
			screen_offset += VGA_WIDTH;//(((new_position-VGA_AREA)/VGA_WIDTH)+1) * VGA_WIDTH;
			new_position = new_position % VGA_WIDTH + VGA_AREA - VGA_WIDTH;

		}
	}
	else if (new_position < 0) {
		// Check if new_position is outside the screen_buffer bounds
		if (screen_offset + new_position < 0) {
			// Bound the screen_offset and Update VGA Memory to reflect
			screen_offset = 0;
			new_position = VGA_WIDTH + new_position; //%VGA_WIDTH
			
		}
		else {
			// Slide the screen_offset and Update VGA Memory to reflect
			screen_offset += (((new_position+1)/VGA_WIDTH)-1) * VGA_WIDTH; 
			new_position = ((new_position+1)%VGA_WIDTH) - 1 + VGA_WIDTH; //VGA_WIDTH + (new_position+1) % VGA_WIDTH - 1;//-1 * new_position % VGA_WIDTH; 
			
		}

	}
	memcpy ( (char*)(screen_buffer + screen_offset), VGA_MEMORY, VGA_AREA*2);


	// Send position to screen
	out_8(REG_SCREEN_CTRL, 0x0f); // cursor offset low byte
	out_8(REG_SCREEN_DATA, (uint8_t)(new_position & 0xff));
	out_8(REG_SCREEN_CTRL, 0x0e); // cursor offset high byte
	out_8(REG_SCREEN_DATA, (uint8_t)((new_position >> 8) & 0xff));

	//Update global position variable
	cursor_position = (uint16_t) new_position;
}

void set_cursor_position_old(sint16_t position) {
	if (position >= VGA_AREA) {
		// save updated terminal to buffer 
		memcpy(VGA_MEMORY,(char*)(screen_buffer+screen_offset), VGA_AREA*2);
		
		if (screen_offset < (SCREEN_BUF_SIZE - VGA_AREA)) {
			// set terminal screen to shift within buffer
			screen_offset += VGA_WIDTH;
		}
		else {
			// grab updated terminal window, lose oldest line and add a new line to buffer
			memcpy((char*)(screen_buffer+VGA_WIDTH), (char*)(screen_buffer), SCREEN_BUF_SIZE*2 - VGA_WIDTH*2);
			clear_buf_line(BACKGROUND_DEFAULT | FOREGROUND_DEFAULT, SCREEN_BUF_SIZE/VGA_WIDTH -1);
		}
		// update terminal window based on shift/update of buffer 
		memcpy((char*)(screen_buffer+screen_offset), (char*)(VGA_MEMORY), VGA_AREA*2);
		position = position - VGA_WIDTH;
	}
	else if (position < 0 ) {
		// save updated terminal to buffer 
		memcpy(VGA_MEMORY,(char*)(screen_buffer+screen_offset), VGA_AREA*2);
		if (screen_offset >= VGA_WIDTH) {
			// set terminal screen to shift within buffer
			screen_offset -= VGA_WIDTH;
		}
		// grab updated terminal window (possibly shifted) 
		memcpy((char*)(screen_buffer + screen_offset), (char*)VGA_MEMORY, VGA_AREA*2);
		position = position + VGA_WIDTH;
	}

	// Send position to screen
	out_8(REG_SCREEN_CTRL, 0x0f); // cursor offset low byte
	out_8(REG_SCREEN_DATA, (uint8_t)(position & 0xff));
	out_8(REG_SCREEN_CTRL, 0x0e); // cursor offset high byte
	out_8(REG_SCREEN_DATA, (uint8_t)((position >> 8) & 0xff));

	//Update global position variable
	cursor_position = (uint16_t) position;
}

void clear_buf_line(uint64_t color, uint16_t line) {
	uint64_t reg = 0;
	for (int i=0; i<4; i++) {
		reg += color << (8 + 16*i);
	}
	// Fill the VGA_AREA 
	uint64_t* ptr = (uint64_t*) screen_buffer+(VGA_WIDTH*line/4);
	for (int i=0; i<VGA_WIDTH/4; i++) {
		*ptr = reg; 
		ptr++;
	}
}
void clear_line(uint64_t color, uint16_t line) {
	uint64_t reg = 0;
	for (int i=0; i<4; i++) {
		reg += color << (8 + 16*i);
	}
	// Fill the VGA_AREA 
	uint64_t* ptr = (uint64_t*) VGA_MEMORY+VGA_WIDTH*line/4;
	for (int i=0; i<VGA_WIDTH/4; i++) {
		*ptr = reg; 
		ptr++;
	}
}

void clear_screen (uint64_t color) {
	uint64_t reg = 0;

	// Fill the register w/ 64bits of clear code   
	for (int i=0; i<4; i++) {
		reg += color << (8 + 16*i);
	}

	// Fill the VGA_AREA 
	uint64_t* ptr = (uint64_t*) VGA_MEMORY;
	for (int i=0; i<VGA_AREA/4; i++) {
		*ptr = reg; 
		ptr++;
	}
	set_cursor_position(0);
}

void init_screen (uint64_t color) {
	uint64_t reg = 0;

	// Fill the register w/ 64bits of clear code   
	for (int i=0; i<4; i++) {
		reg += color << (8 + 16*i);
	}

	// Fill the screen_buffer
	uint64_t* ptr = (uint64_t*) screen_buffer;
	for (int i=0; i<SCREEN_BUF_SIZE/4; i++) {
		*ptr = reg; 
		ptr++;
	}

	screen_offset = 0;
	//BufferPosition = 0;
	set_cursor_position(0);

	memcpy((char*)screen_buffer, (char*)VGA_MEMORY, VGA_AREA*2);
}

uint16_t position_from_coords(uint8_t x, uint8_t y) {
	return y*VGA_WIDTH + x;
}

uint8_t x_coord_from_position(uint16_t position) {
	return position % VGA_WIDTH;
}

uint8_t y_coord_from_position(uint16_t position) {
	return (position / VGA_WIDTH) - (position % VGA_WIDTH);
}


void print_string(const char* str) {
	uint8_t* char_ptr = (uint8_t*)str;
	uint16_t index = cursor_position;
	uint16_t tmp;
	uint16_t prev;
	uint8_t new_line = 0;
	while (*char_ptr != 0) {
		switch (*char_ptr) {
			case 10:
				index += VGA_WIDTH;
				break;
			case 13:
				index -= (index % VGA_WIDTH);
				break;
			default:
				prev = *(screen_buffer + screen_offset + index) & 0xff00;
				tmp = (uint16_t) *char_ptr & 0x00ff;
				*(screen_buffer + screen_offset + index) = prev | tmp;
				index++;
				if (index % VGA_WIDTH == 0) { new_line = 1;}
				break;
		}
		char_ptr++;

		if (new_line == 1) {
			set_cursor_position(index);
			new_line = 0;
		}
	}	
	set_cursor_position(index);
}

void print_string_old (const char* str) {
	uint8_t* char_ptr = (uint8_t*)str;
	uint16_t index = cursor_position;
	while (*char_ptr != 0) {
		switch (*char_ptr) {
			case 10:
				index += VGA_WIDTH;
				break;
			case 13:
				index -= (index % VGA_WIDTH);
				break;
			default: 
				*(VGA_MEMORY + index * 2) = *char_ptr;
				index++;
		}
		char_ptr++;
	}

	set_cursor_position(index);
}

void memset(uint64_t val, uint64_t* dest, uint64_t num) {
	if (1) {
		uint8_t* val_byte = (uint8_t*)&val;
		for(uint8_t* dest_byte = (uint8_t*)dest; dest_byte < (uint8_t*)(uint64_t)dest + num; dest_byte++) {
			*dest_byte = *val_byte;
		}
		return;
	}
}

/* Copy bytes from one place to another. */
void memcpy(char* src, char* dst, int len) {
	for (int i=0; i<len; i++) {
		*(dst+i) = *(src+i);
	} 
}

/* Reverse memcpy*/
void memcpy_rev  (char* src, char* dst, int len) {
	for (int i=0; i<len; i++) {
		*(dst+len-1-i) = *(src+len-1-i);
	}
}

void print_char_old (char chr) {
	uint16_t index = cursor_position;
	*(VGA_MEMORY + index * 2) = chr;
	set_cursor_position(++index);
}

void print_char (char chr) {
	uint16_t index = cursor_position;
	uint16_t prev = *(screen_buffer + screen_offset + index) & 0xff00;
	uint16_t tmp = (uint16_t) chr & 0x00ff;
	*(screen_buffer + screen_offset + index) = prev | tmp;
	set_cursor_position(++index);
}

uint64_t hex_len(uint64_t val) {
	uint64_t sz = 1;
	while(val/16 > 0) {
		sz++;
		val/=16;
	}
	return sz;
}

char hex_to_str_buf[128];
char* hex_to_str(uint64_t val) {
	*hex_to_str_buf = 0;
	uint8_t* ptr;
	uint8_t tmp;
	uint64_t sz = hex_len(val);
	sz = (sz%2==0) ? sz/2 : (sz+1)/2; //pad to byte width
	for (int i=0; i<sz; i++) {
		ptr = (uint8_t*) &val + sz - i - 1;
		tmp = (*ptr & 0xf0) >> 4;
		hex_to_str_buf[2+i*2] = tmp + (tmp > 9 ? 87 : 48);
		tmp = (*ptr & 0x0f);
		hex_to_str_buf[2+i*2+1] = tmp + (tmp > 9 ? 87 : 48);
	}
	hex_to_str_buf[2+sz*2] = '\0';
	hex_to_str_buf[0] = '0';
	hex_to_str_buf[1] = 'x';
	return hex_to_str_buf;
}

char int_to_str_buf[128];
char* int_to_str(uint64_t val) {
	int sz = 1; 
    int tmp = val;
    while ((tmp/10) > 0) {
        sz++;
        tmp /= 10;
    }

	*int_to_str_buf = 0;
	for (int i=0; i<sz; i++) {
		uint8_t r = val % 10;
		int_to_str_buf[sz-i-1] = r + 48;
		val /= 10;
	}

	int_to_str_buf[sz] = '\0';
	return int_to_str_buf;
}

char float_to_str_buf_whole[128];
char float_to_str_buf_frac[128];

char* float_to_str(double val, uint8_t percision) {
	uint64_t whole = (uint64_t)val;
	print_string(int_to_str(whole));

	double frac = val - whole;

	float_to_str_buf_frac[0] = '.';

	uint64_t digit;
	uint8_t i;

	for (i=0; i < percision; i++) {
		frac = frac * 10;
		digit = (uint64_t)frac;
		float_to_str_buf_frac[i+1] = digit+48;
		frac = frac - digit;
	}
	return float_to_str_buf_frac;
}


void print_any(void* ptr, int sz, int typ) {
	switch (typ) {
		case STR:
			print_string((char*)*((char**)ptr));
			break;
		case CHR:
			print_char((char)*((char*)ptr));
			break;
		case DEC:
			print_string(int_to_str((int)*((int*)ptr)));
			break;
		case HEX:
			print_string(hex_to_str( (int)(*((int*)ptr)) ));
			break;
		case FLT:
			print_string("Float");
			break;
		default:
			break;
	}
}





