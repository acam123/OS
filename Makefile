CC = x86_64-elf-gcc
CFLAGS = -Ttext 0x8000 -ffreestanding -mno-red-zone -m64
#CFLAGS = -ffreestanding -mno-red-zone -m64

OBJS = kernel.o inc/IDT.o inc/IO.o inc/KeyboardHandler.o inc/MemoryMap.o inc/TextPrint.o inc/KeyboardScanCodeSet.o inc/Bitmap.o inc/PageAllocator.o inc/PageTable.o inc/Heap.o

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
	qemu-system-x86_64 -fda combined.iso -drive file=AidanOS.img -m 512M

.PHONY : clean
clean :
	-rm *.o *.bin inc/*.o

