#include "Heap.h"

free_node* free_head;
uint64_t heap_start = 0x100000000000;
uint64_t heap_size;

free_node* init_heap(/*uint64_t heapStart, uint64_t heapLen*/) {
	map_memory(heap_start, (uint64_t)request_page(), (page_table*)0x1000 );
	free_node* heap_head = (free_node*) heap_start;//request_page();
	heap_size = PAGE_SIZE;
	heap_head->sz = heap_size - sizeof(free_node);
	heap_head->next = (free_node*)0x0;
	free_head = (free_node*) heap_head;
	return heap_head;
}

void* malloc (uint64_t sz) {
	void* new_mem_ptr = (void*) 0x0;

	// Minimum size malloc returns must be large enough to be freed
	if ( sz + sizeof(mem_seg_header) < sizeof(free_node) ) {
		sz = sizeof(free_node) - sizeof(mem_seg_header);
	}

	free_node* this_node = free_head;
	// Iterate through the freelist for a large enough segment
	while ( (uint64_t)this_node != 0x0 ) {
		// Found suitable block
		if ( this_node->sz > (sz + sizeof(mem_seg_header)) ) {
			// Duplicate free_node and update size
			free_node* new_free_node = (free_node*) (((uint64_t)this_node) + sizeof(mem_seg_header) + sz);
			new_free_node->sz = this_node->sz - sz - sizeof(mem_seg_header);
			new_free_node->next = this_node->next;

			// Cast Old Free Node to New Memory Segment Header and fill in
			memset(0, (uint64_t*)this_node, sizeof(free_node));
			mem_seg_header* new_mem_header = (mem_seg_header*) this_node;
			new_mem_header->sz = sz;
			new_mem_header->magic = MAGIC_NUM;

			// Return pointer to the start of the New Memory Segment (after the header)
			new_mem_ptr = (void*) ( ((uint64_t) new_mem_header) + sizeof(mem_seg_header) ); 
			
			// Update the Global free_head pointer
			if (this_node = free_head) {
				free_head = new_free_node;
			}

			break;
		}
		this_node = this_node->next;
	} 

	// Large enough segment not found
	if ( (uint64_t)new_mem_ptr == 0x0 ) {
		// get more page(s)
		uint64_t remainder = (sz + sizeof(mem_seg_header) + sizeof(free_node)) % 4096;
		uint64_t num_pages = (sz + sizeof(mem_seg_header) + sizeof(free_node)) / 4096;
		if (remainder > 0) {
			num_pages++;
		}

		for (uint64_t i=0; i<num_pages; i++) {
			map_memory(heap_start + heap_size + i*PAGE_SIZE, (uint64_t)request_page(), (page_table*)0x1000);
		}

		// Fill in Memory Segment Header
		mem_seg_header* new_mem_header = (mem_seg_header*) (heap_start + heap_size);
		new_mem_header->sz = sz;
		new_mem_header->magic = MAGIC_NUM;

		// Return pointer to the start of the New Memory Segment (after the header)
		new_mem_ptr = (void*) (heap_start + heap_size + sizeof(mem_seg_header));

		free_node* new_free_node = (free_node*) ( (uint64_t)new_mem_ptr + sz);
		new_free_node->sz = PAGE_SIZE - (uint64_t)new_free_node - sizeof(free_node); //???
		new_free_node->next = free_head;
		free_head = new_free_node;

		heap_size += num_pages*PAGE_SIZE;
	}
	return new_mem_ptr;
}

void free (void* addr) {
	mem_seg_header* mem_header = (mem_seg_header*) ((uint64_t)addr - sizeof(mem_seg_header));
	uint64_t sz = mem_header->sz;
	if (mem_header->magic == MAGIC_NUM) {
		memset(0, (uint64_t*) mem_header, sz + sizeof(mem_seg_header));
		free_node* newfree_node = (free_node*) mem_header;
		newfree_node->sz = sz + sizeof(mem_seg_header) - sizeof(free_node);

		newfree_node->next = free_head; 
		free_head = newfree_node; 

	}
	return;
}

void print_free_list() {
	print_string("\n\rHEAD: ");
	print_string(hex_to_str((uint64_t)free_head));

	free_node* thisNode = free_head;
	// Iterate through the freelist 
	while ( (uint64_t)thisNode != 0x0 ) {
		print_string("\n\rFree Node: ");
		print_string(hex_to_str((uint64_t)thisNode));
		print_string("\n\r - sz: ");
		print_string(hex_to_str(thisNode->sz));
		print_string("\n\r - next: ");
		print_string(hex_to_str((uint64_t)thisNode->next));
		thisNode = thisNode->next;
	}
	return;
}

uint8_t coalesce(free_node* node) {
	uint8_t out = 0;
	if ( (uint64_t)node->next == (uint64_t)node + node->sz + sizeof(free_node) ) {
			node->sz += node->next->sz + sizeof(free_node);
			free_node* tmpPtr = node->next;
			node->next = node->next->next;
			memset(0, (uint64_t*) tmpPtr, sizeof(free_node));
			out = 1;
	}
	return out;
}

free_node* split_list(free_node* list, free_node** l, free_node** r) {
	free_node* mid = list;
	free_node* tail = list;

	while((uint64_t)tail->next != 0x0) {
		tail = tail->next;
		if ((uint64_t)tail->next != 0x0) {
			tail = tail->next;
			mid = mid->next;
		}
	}

	*l = list;
	*r = mid->next;
	mid->next = (free_node*)0x0;
	return *r; 
}

free_node* merge(free_node* l, free_node* r) {
	free_node* out;
	if ( (uint64_t)l == 0x0 ) {
		out = r;
	}
	else if ( (uint64_t)r == 0x0) {
		out = l;
	}
	else if ( (uint64_t) r < (uint64_t) l ) {
		out = r;
		out->next = merge(l, r->next);
	} 
	else {
		out = l;
		out->next = merge(l->next, r);
	}
	return out;

}

free_node* merge_sort(free_node** nodePtr) {
	if ( (uint64_t)(*nodePtr) == 0x0 || (uint64_t)(*nodePtr)->next == 0x0 )
	{
		return *nodePtr;
	}

	free_node* l;
	free_node* r;
	split_list(*nodePtr, &l, &r);

	merge_sort(&l);
	merge_sort(&r);
	*nodePtr = merge(l,r);
	return *nodePtr;

}

void sort_free_list() {
	merge_sort(&free_head);
}

void coalesce_all () {
	sort_free_list();

	free_node* thisNode = free_head;
	while ( (uint64_t) thisNode != 0x0) {
		uint8_t ret = coalesce(thisNode);
		if (ret == 0) {
			thisNode = thisNode->next;
		}
	}
	return;
} 

void print_list(free_node* list) {
	while ((uint64_t)list != 0x0) {
		print_string("\n\rNode: ");
		print_string(hex_to_str((uint64_t)list));
		print_string("\n\r - sz: ");
		print_string(hex_to_str(list->sz));
		print_string("\n\r - next: ");
		print_string(hex_to_str((uint64_t)list->next));
		list = list->next;
	}
}


