#ifndef _HEAP_
#define _HEAP_


#include "Typedefs.h"
#include "TextPrint.h"
#include "PageTable.h"
#include "PageAllocator.h"

#define MAGIC_NUM 123456789

typedef struct _free_node {
	uint64_t sz;
	struct _free_node* next;
} free_node;


typedef struct {
    uint64_t sz;
	uint64_t magic; 
} mem_seg_header;

extern free_node* free_head;
extern uint64_t heap_size;

free_node* init_heap(/*uint64_t heapStart, uint64_t heapLen*/);
void* malloc (uint64_t sz);
void free (void* addr);
void* calloc (uint64_t sz);
void* realloc(uint64_t sz);
void print_free_list();
uint8_t coalesce();
void coalesce_all ();
void sort_free_list();
free_node* merge_sort(free_node** list);
free_node* merge(free_node* l, free_node* r);
free_node* split_list(free_node* list, free_node** l, free_node** r);
void print_list(free_node* list);

#endif
