[org 0x7c00]
[bits 16]

mov [BOOT_DISK], dl;; store the disk number 

mov bp,0x7c00;; initialize base pointer *maybe 0x9000 or 0x7BFF
mov sp, bp;; initialize stack pointer to bootloader origin

mov bx, InitializationString;; set bx to point to string data location
call print_string

;;mov bx, 0x1fb6
;;call PrintHex

call ReadDisk

jmp EXTENDED_SPACE

%include "inc/PrintString.asm"
%include "inc/PrintHex.asm"
%include "inc/DiskRead.asm" 

InitializationString:
	db '...AidanOS Initializing...',0

times 510-($-$$) db 0;; fill rest of bootloader with 00's
dw 0xaa55;; end with magic number for BIOS to recognize as bootloader