CC = x86_64-elf-gcc
CFLAGS = -Ttext 0x8000 -ffreestanding -mno-red-zone -m64
#CFLAGS = -ffreestanding -mno-red-zone -m64

SRCS = $(wildcard inc/*.c) kernel.c
OBJS = $(SRCS:.c=.o)
#OBJS = kernel.o inc/IDT.o inc/IO.o inc/keyboard_handler.o inc/MemoryMap.o inc/TextPrint.o inc/KeyboardScanCodeSet.o inc/Bitmap.o inc/PageAllocator.o inc/page_table.o inc/Heap.o inc/Mouse.o


all : AidanOS.iso 

mbr.bin : mbr.asm
	nasm mbr.asm -f bin -o $@

boot.o : boot.asm
	nasm boot.asm -f elf64 -o $@

$(OBJS): %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

kernel.bin : boot.o $(OBJS)
	x86_64-elf-ld -T"linker.ld"

AidanOS.iso : mbr.bin kernel.bin
	cat mbr.bin > $@
	cat kernel.bin >> $@
	printf '\x00%.0s' {1..524288} >> $@ # fill end with 00's to match the max partition size in mbr

run : AidanOS.iso
	#qemu-system-x86_64 -drive file=combined.iso,format=raw -drive file=AidanOS.img -m 512M -usb -device usb-host,vendorid=0x13fe,productid=0x6300
	#qemu-system-x86_64 -drive file=combined.iso,format=raw -drive file=AidanOS.img -m 512M -device usb-ehci -device usb-host,vendorid=0x13fe,productid=0x6300
	#qemu-system-x86_64 -drive file=combined.iso,format=raw -drive file=AidanOS.img -m 512M -device usb-ehci 
	#qemu-system-x86_64 -boot menu=on -drive file=combined.iso,format=raw,id=test_id -drive file=AidanOS.img -m 512M 
	qemu-system-x86_64 -machine q35 -bios /usr/local/Cellar/qemu/5.2.0_1/share/qemu/bios.bin -drive file=AidanOS.iso,format=raw -m 512M

.PHONY : clean
clean :
	-rm *.o *.bin inc/*.o

