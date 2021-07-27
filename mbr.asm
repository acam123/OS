[org 0x7c00]
[bits 16]

mov ax, 0
mov ss, ax
mov bp, 0x7c00 ;; initialize base pointer *maybe 0x9000 or 0x7BFF
mov sp, bp ;; initialize stack pointer to bootloader origin

mov ds, ax
mov [BOOT_DISK], dl;; store the disk number 

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


times 446-($-$$) db 0;; include 0's before partition table

db 0x80 ;; active,
db 0x00 ;; head=0,
db 0x01 ;; sector=1(6 bits)
db 0x00 ;; cylinder=0  
 
db 0x7f ;; custom os
db 0xfe ;; head=254 (max)
db 0xff ;; sector=63 (max)
db 0xff ;; cylinder=0x3ff (max)

db 0x01 ;; partition starts at 1st sector
db 0x00 
db 0x00 
db 0x00 

db 0xff ;; OS is less than 255 sectors
db 0x00 
db 0x00 
db 0x00

times 510-($-$$) db 0 ;; fill rest with 0's

dw 0xaa55;; end with magic number for BIOS to recognize as mbr