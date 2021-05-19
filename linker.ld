OUTPUT_FORMAT(binary)

ENTRY (_start)

INPUT
(
	extended.o
	kernel.o
	inc/IDT.o
	inc/IO.o
	inc/KeyboardHandler.o
	inc/MemoryMap.o
	inc/TextPrint.o
	inc/KeyboardScanCodeSet.o
)

OUTPUT
(
	kernel.bin
)

SECTIONS
{
	. = 0x8000;

	.text : ALIGN(0x1000)
	{
		*(.text)
	}

	.idt BLOCK(0x1000) : ALIGN(0x1000)
	{
		_idt = .;
		. = . + 0x1000;
	}

	.data : ALIGN(0x1000)
	{
		*(.data)
	}

	.rodata : ALIGN(0x1000)
	{
		*(.rodata)
	}

	.bss : ALIGN(0x1000)
	{
		*(COMMON)
		*(.bss)
	}


}