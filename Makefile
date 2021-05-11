all : LoadBins.o combined.iso 

LoadBins.o : LoadBins.asm
	nasm LoadBins.asm -f elf64 -o $@

bootloader.bin : bootloader.asm
	nasm bootloader.asm -f bin -o $@

extended.o : extended.asm
	nasm extended.asm -f elf64 -o $@

kernel.o : kernel.c inc/TextPrint.c
	x86_64-elf-gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "kernel.c" -o $@

kernel.bin : extended.o kernel.o
	x86_64-elf-ld -T"linker.ld"

combined.iso : bootloader.bin kernel.bin
	cat bootloader.bin > $@
	cat kernel.bin >> $@

run : combined.iso AidanOS.img
	qemu-system-x86_64 -fda combined.iso -drive file=AidanOS.img -m 512M
