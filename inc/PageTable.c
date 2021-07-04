#include "PageTable.h"

/* NOTE!!! I think virtual addresses all have to just use the bottom 48 bits so
 * maybe need to remove top 16 on first PML4 request??? each time??? 
 */
void CalcPageMapIndex (uint_64 virtAddr, PageMapIndex* PageIndex) {
    PageIndex->Offset = virtAddr & 0xfff;
    virtAddr >>= 12;
    PageIndex->P_i = virtAddr & 0x1ff;
    virtAddr >>= 9;
    PageIndex->PT_i = virtAddr & 0x1ff;
    virtAddr >>= 9;
    PageIndex->PD_i = virtAddr & 0x1ff;
    virtAddr >>= 9;
    PageIndex->PDP_i = virtAddr & 0x1ff;
    //Note top 16 bits of virtual address not used 
}

void* MapMemoryRec (uint_64 virtAddr, PageTable* Table) {
	uint_64 mask;
	uint_16 index;
	uint_8 PTDepth = 4;
	for (uint_8 i=0; i<PTDepth; i++) {
		mask = 0x0000ff8000000000 >> 9*i; // first 16 bits not used
		index = (virtAddr & mask) >> (12 + 9*(PTDepth-1-i)); 
		Table = PageTableCrawl(index, Table);
	}
	void* Page = (void*) Table;
	return Page;
}



void MapMemory(uint_64 virtAddr, uint_64 physAddr, PageTable* PML_4) {
	PageMapIndex pIndex;
	CalcPageMapIndex(virtAddr, &pIndex);
		
	PageTable* PDP = PageTableCrawl(pIndex.PDP_i, PML_4);

	PageTable* PD = PageTableCrawl(pIndex.PD_i, PDP);

	PageTable* PT = PageTableCrawl(pIndex.PT_i, PD);

	PageTableEntry PTE = (PageTableEntry) PT->entries[pIndex.P_i];

	PTE.Address = (uint_64) physAddr >> 12;
	PTE.Present = 0b1;
	PTE.ReadWrite = 0b1;
	PT->entries[pIndex.P_i] = PTE;
	return;
}

PageTable* PageTableCrawl (uint_64 index, PageTable* Table ) {

	PageTableEntry PTE = (PageTableEntry) Table->entries[index]; 
	PageTable* NextPageTable;
	if (PTE.Present == 0b1) {
		NextPageTable = (PageTable*) ((uint_64) PTE.Address << 12);
	} 
	else {
		//Create
		NextPageTable = (PageTable*) requestPage();
		for (int i=0; i<PAGE_SIZE; i++) {
			*((uint_64*)((uint_64)NextPageTable+i)) = 0b00000000;
		}

		PTE.Address = (uint_64)NextPageTable >> 12;
		PTE.Present = 0b1;
		PTE.ReadWrite = 0b1;
		Table->entries[index] = PTE;
	}
	return NextPageTable; 
}

void printPageTableEntry(PageTableEntry PTE) {
	PrintString("\n\r");
	PrintString("Present:");
	PrintString(IntToStr((uint_8) PTE.Present));
	PrintString("\n\r");
	PrintString("ReadWrite:");
	PrintString(IntToStr((uint_8) PTE.ReadWrite));
	PrintString("\n\r");
	PrintString("UserSuper:");
	PrintString(IntToStr((uint_8) PTE.UserSuper));
	PrintString("\n\r");
	PrintString("WriteThrough:");
	PrintString(IntToStr((uint_8) PTE.WriteThrough));
	PrintString("\n\r");
	PrintString("CacheDisabled:");
	PrintString(IntToStr((uint_8) PTE.CacheDisabled));
	PrintString("\n\r");
	PrintString("Accessed:");
	PrintString(IntToStr((uint_8) PTE.Accessed));
	PrintString("\n\r");
	PrintString("ignore0:");
	PrintString(IntToStr((uint_8) PTE.ignore0));
	PrintString("\n\r");
	PrintString("LargerPages:");
	PrintString(IntToStr((uint_8) PTE.LargerPages));
	PrintString("\n\r");
	PrintString("ignore1:");
	PrintString(IntToStr((uint_8) PTE.ignore1));
	PrintString("\n\r");
	PrintString("Available:");
	PrintString(IntToStr((uint_8) PTE.Available));
	PrintString("\n\r");
	PrintString("Address:");
	PrintString(HexToStr((uint_64) PTE.Address));
	PrintString("\n\r");
}

void extendIdentityMap() {
	/* 
    * SimplePaging.asm identity mapped up to 0x200000;
    * PML4 = 0x1000 (can we grab from a global variable instead???)
    * Then map rest of address space to 512MB (0x20000000)
    * (maybe get max automatically instead???)
    */
    PageTable* PML4 = (PageTable*)0x1000;
    uint_64 NewPageAddr = 0x200000; // End of 1st Page of PT made in assembly
    for (uint_64 i=0; i<0x1fe00; i++) {
    	MapMemory(NewPageAddr + i*PAGE_SIZE, NewPageAddr + i*PAGE_SIZE, PML4);
    }
}