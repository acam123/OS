OUTPUT_FORMAT(binary)

ENTRY (_start)

INPUT
(
	boot.o
	kernel.o
	inc/IDT.o
	inc/IO.o
	inc/KeyboardHandler.o
	inc/MemoryMap.o
	inc/TextPrint.o
	inc/KeyboardScanCodeSet.o
	inc/Bitmap.o
	inc/PageAllocator.o
	inc/PageTable.o
	inc/Heap.o
	inc/Mouse.o
	inc/PCI.o
	inc/PIT.o
	inc/Serial.o
	inc/PCIClassCodes.o
	inc/AHCI.o
	inc/Process.o
	inc/UHCI.o
	inc/USB.o

)

OUTPUT
(
	kernel.bin
)

SECTIONS
{
	. = 0x8000;

	_kernel_start = .;

	.text : ALIGN(0x1000)
	{
		_text_sect = .;
		*(.text)
	}

	.idt BLOCK(0x1000) : ALIGN(0x1000)
	{
		_idt = .; 
		. = . + 0x1000;
	}

	.data : ALIGN(0x1000)
	{
		_data_sect = .;
		*(.data)
	}

	.rodata : ALIGN(0x1000)
	{
		_rodata_sect = .;
		*(.rodata)
	}

	.bss : ALIGN(0x1000)
	{
		_bss_sect = .;
		*(COMMON)
		*(.bss)
	}

	_kernel_end = .;

}