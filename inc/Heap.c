#include "Heap.h"

FreeNode* FreeHead;
uint_64 HeapStart = 0x100000000000;
uint_64 HeapSize;

FreeNode* InitHeap(/*uint_64 heapStart, uint_64 heapLen*/) {
	MapMemory(HeapStart, (uint_64)requestPage(), (PageTable*)0x1000 );
	FreeNode* HeapHead = (FreeNode*) HeapStart;//requestPage();
	HeapSize = PAGE_SIZE;
	HeapHead->sz = HeapSize - sizeof(FreeNode);
	HeapHead->next = (FreeNode*)0x0;
	FreeHead = (FreeNode*) HeapHead;
	return HeapHead;
}

void* malloc (uint_64 sz) {
	void* NewMemPtr = (void*) 0x0;

	// Minimum size malloc returns must be large enough to be freed
	if ( sz + sizeof(MemSegHeader) < sizeof(FreeNode) ) {
		sz = sizeof(FreeNode) - sizeof(MemSegHeader);
	}

	FreeNode* thisNode = FreeHead;
	// Iterate through the freelist for a large enough segment
	while ( (uint_64)thisNode != 0x0 ) {
		// Found suitable block
		if ( thisNode->sz > (sz + sizeof(MemSegHeader)) ) {
			// Duplicate FreeNode and update size
			FreeNode* NewFreeNode = (FreeNode*) (((uint_64)thisNode) + sizeof(MemSegHeader) + sz);
			NewFreeNode->sz = thisNode->sz - sz - sizeof(MemSegHeader);
			NewFreeNode->next = thisNode->next;

			// Cast Old Free Node to New Memory Segment Header and fill in
			memset(0, (uint_64*)thisNode, sizeof(FreeNode));
			MemSegHeader* NewMemHeader = (MemSegHeader*) thisNode;
			NewMemHeader->sz = sz;
			NewMemHeader->magic = MAGIC_NUM;

			// Return pointer to the start of the New Memory Segment (after the header)
			NewMemPtr = (void*) ( ((uint_64) NewMemHeader) + sizeof(MemSegHeader) ); 
			
			// Update the Global FreeHead pointer
			if (thisNode = FreeHead) {
				FreeHead = NewFreeNode;
			}

			break;
		}
		thisNode = thisNode->next;
	} 

	// Large enough segment not found
	if ( (uint_64)NewMemPtr == 0x0 ) {
		// get more page(s)
		uint_64 remainder = (sz + sizeof(MemSegHeader) + sizeof(FreeNode)) % 4096;
		uint_64 numPages = (sz + sizeof(MemSegHeader) + sizeof(FreeNode)) / 4096;
		if (remainder > 0) {
			numPages++;
		}

		for (uint_64 i=0; i<numPages; i++) {
			MapMemory(HeapStart + HeapSize + i*PAGE_SIZE, (uint_64)requestPage(), (PageTable*)0x1000);
		}

		// Fill in Memory Segment Header
		MemSegHeader* NewMemHeader = (MemSegHeader*) (HeapStart + HeapSize);
		NewMemHeader->sz = sz;
		NewMemHeader->magic = MAGIC_NUM;

		// Return pointer to the start of the New Memory Segment (after the header)
		NewMemPtr = (void*) (HeapStart + HeapSize + sizeof(MemSegHeader));

		FreeNode* NewFreeNode = (FreeNode*) ( (uint_64)NewMemPtr + sz);
		NewFreeNode->sz = PAGE_SIZE - (uint_64)NewFreeNode - sizeof(FreeNode); //???
		NewFreeNode->next = FreeHead;
		FreeHead = NewFreeNode;

		HeapSize += numPages*PAGE_SIZE;
	}
	return NewMemPtr;
}

void free (void* addr) {
	MemSegHeader* memHeader = (MemSegHeader*) ((uint_64)addr - sizeof(MemSegHeader));
	uint_64 sz = memHeader->sz;
	if (memHeader->magic == MAGIC_NUM) {
		memset(0, (uint_64*) memHeader, sz + sizeof(MemSegHeader));
		FreeNode* newFreeNode = (FreeNode*) memHeader;
		newFreeNode->sz = sz + sizeof(MemSegHeader) - sizeof(FreeNode);

		newFreeNode->next = FreeHead; 
		FreeHead = newFreeNode; 

	}
	return;
}

void printFreeList() {
	PrintString("\n\rHEAD: ");
	PrintString(HexToStr((uint_64)FreeHead));

	FreeNode* thisNode = FreeHead;
	// Iterate through the freelist 
	while ( (uint_64)thisNode != 0x0 ) {
		PrintString("\n\rFree Node: ");
		PrintString(HexToStr((uint_64)thisNode));
		PrintString("\n\r - sz: ");
		PrintString(HexToStr(thisNode->sz));
		PrintString("\n\r - next: ");
		PrintString(HexToStr((uint_64)thisNode->next));
		thisNode = thisNode->next;
	}
	return;
}

uint_8 coalesce(FreeNode* node) {
	uint_8 out = 0;
	if ( (uint_64)node->next == (uint_64)node + node->sz + sizeof(FreeNode) ) {
			node->sz += node->next->sz + sizeof(FreeNode);
			FreeNode* tmpPtr = node->next;
			node->next = node->next->next;
			memset(0, (uint_64*) tmpPtr, sizeof(FreeNode));
			out = 1;
	}
	return out;
}

FreeNode* splitList(FreeNode* list, FreeNode** l, FreeNode** r) {
	FreeNode* mid = list;
	FreeNode* tail = list;

	while((uint_64)tail->next != 0x0) {
		tail = tail->next;
		if ((uint_64)tail->next != 0x0) {
			tail = tail->next;
			mid = mid->next;
		}
	}

	*l = list;
	*r = mid->next;
	mid->next = (FreeNode*)0x0;
	return *r;
}

FreeNode* merge(FreeNode* l, FreeNode* r) {
	FreeNode* out;
	if ( (uint_64)l == 0x0 ) {
		out = r;
	}
	else if ( (uint_64)r == 0x0) {
		out = l;
	}
	else if ( (uint_64) r < (uint_64) l ) {
		out = r;
		out->next = merge(l, r->next);
	} 
	else {
		out = l;
		out->next = merge(l->next, r);
	}
	return out;

}

FreeNode* mergeSort(FreeNode** nodePtr) {
	if ( (uint_64)(*nodePtr) == 0x0 || (uint_64)(*nodePtr)->next == 0x0 )
	{
		return *nodePtr;
	}

	FreeNode* l;
	FreeNode* r;
	splitList(*nodePtr, &l, &r);

	mergeSort(&l);
	mergeSort(&r);
	*nodePtr = merge(l,r);
	return *nodePtr;

}

void sortFreeList() {
	mergeSort(&FreeHead);
}

void coalesceAll () {
	sortFreeList();

	FreeNode* thisNode = FreeHead;
	while ( (uint_64) thisNode != 0x0) {
		uint_8 ret = coalesce(thisNode);
		if (ret == 0) {
			thisNode = thisNode->next;
		}
	}
	return;
} 

void printList(FreeNode* list) {
	while ((uint_64)list != 0x0) {
		PrintString("\n\rNode: ");
		PrintString(HexToStr((uint_64)list));
		PrintString("\n\r - sz: ");
		PrintString(HexToStr(list->sz));
		PrintString("\n\r - next: ");
		PrintString(HexToStr((uint_64)list->next));
		list = list->next;
	}
}


