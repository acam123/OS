#ifndef _MOUSE_ 
#define _MOUSE_ 

#include "IO.h"
#include "Typedefs.h"

void init_ps2_mouse ();
void mouse_wait();
void mouse_wait_input();
uint8_t mouse_read();
void mouse_write(uint8_t val);

#endif