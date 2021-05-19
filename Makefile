all : combined.iso 

bootloader.bin : bootloader.asm
	nasm bootloader.asm -f bin -o $@

extended.o : extended.asm
	nasm extended.asm -f elf64 -o $@

kernel.o : kernel.c
	x86_64-elf-gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "kernel.c" -o $@

inc/IDT.o : inc/IDT.c
	x86_64-elf-gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "inc/IDT.c" -o $@

inc/IO.o : inc/IO.c
	x86_64-elf-gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "inc/IO.c" -o $@

inc/KeyboardHandler.o : inc/KeyboardHandler.c
	x86_64-elf-gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "inc/KeyboardHandler.c" -o $@

inc/MemoryMap.o : inc/MemoryMap.c
	x86_64-elf-gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "inc/MemoryMap.c" -o $@

inc/TextPrint.o : inc/TextPrint.c
	x86_64-elf-gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "inc/TextPrint.c" -o $@

inc/KeyboardScanCodeSet.o : inc/KeyboardScanCodeSet.c
	x86_64-elf-gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "inc/KeyboardScanCodeSet.c" -o $@

kernel.bin : extended.o kernel.o inc/IDT.o inc/IO.o inc/KeyboardHandler.o inc/MemoryMap.o inc/TextPrint.o inc/KeyboardScanCodeSet.o
	x86_64-elf-ld -T"linker.ld"

combined.iso : bootloader.bin kernel.bin
	cat bootloader.bin > $@
	cat kernel.bin >> $@

run : combined.iso AidanOS.img
	qemu-system-x86_64 -fda combined.iso -drive file=AidanOS.img -m 512M

.PHONY : clean
clean :
	-rm *.o *.bin inc/*.o
