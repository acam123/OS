CC = x86_64-elf-gcc
CFLAGS = -Ttext 0x8000 -ffreestanding -mno-red-zone -m64
#CFLAGS = -ffreestanding -mno-red-zone -m64

SRCS = $(wildcard inc/*.c) kernel.c
OBJS = $(SRCS:.c=.o)
#OBJS = kernel.o inc/IDT.o inc/IO.o inc/keyboard_handler.o inc/MemoryMap.o inc/TextPrint.o inc/KeyboardScanCodeSet.o inc/Bitmap.o inc/PageAllocator.o inc/page_table.o inc/Heap.o inc/Mouse.o


all : combined.iso 

bootloader.bin : bootloader.asm
	nasm bootloader.asm -f bin -o $@

extended.o : extended.asm
	nasm extended.asm -f elf64 -o $@

$(OBJS): %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

kernel.bin : extended.o $(OBJS)
	x86_64-elf-ld -T"linker.ld"

combined.iso : bootloader.bin kernel.bin
	cat bootloader.bin > $@
	cat kernel.bin >> $@

run : combined.iso AidanOS.img
	#qemu-system-x86_64 -drive file=combined.iso,format=raw -drive file=AidanOS.img -m 512M
	#qemu-system-x86_64 -drive file=combined.iso,format=raw -drive file=AidanOS.img -m 512M -usb -device usb-host,vendorid=0x13fe,productid=0x6300
	qemu-system-x86_64 -drive file=combined.iso,format=raw -drive file=AidanOS.img -m 512M -device usb-ehci -device usb-host,vendorid=0x13fe,productid=0x6300
	#qemu-system-x86_64 -drive file=combined.iso,format=raw -drive file=AidanOS.img -m 512M -device usb-ehci 

.PHONY : clean
clean :
	-rm *.o *.bin inc/*.o

