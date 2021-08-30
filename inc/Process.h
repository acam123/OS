#ifndef _PROCESS_ 
#define _PROCESS_ 

#include "TextPrint.h"

typedef struct _process_control_block {
	uint64_t stack_top;
	uint64_t pml4;
	uint64_t next_task;
	uint64_t state;
	uint64_t priority;
	uint64_t pid;

} process_control_block;

void init_multitasking();

#endif 