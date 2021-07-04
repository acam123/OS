#pragma once
#include "Typedefs.h"
#include "TextPrint.h"
#include "PageTable.h"
#include "PageAllocator.h"

#define MAGIC_NUM 123456789

typedef struct freeNode {
	uint_64 sz;
	struct freeNode* next;
} FreeNode;


typedef struct {
    uint_64 sz;
	uint_64 magic; 
} MemSegHeader;

extern FreeNode* FreeHead;
extern uint_64 HeapSize;

FreeNode* InitHeap(/*uint_64 heapStart, uint_64 heapLen*/);
void* malloc (uint_64 sz);
void free (void* addr);
void* calloc (uint_64 sz);
void* realloc(uint_64 sz);
void printFreeList();
uint_8 coalesce();
void coalesceAll ();
void sortFreeList();
FreeNode* mergeSort(FreeNode** list);
FreeNode* merge(FreeNode* l, FreeNode* r);
FreeNode* splitList(FreeNode* list, FreeNode** l, FreeNode** r);
void printList(FreeNode* list);
