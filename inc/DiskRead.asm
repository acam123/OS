EXTENDED_SPACE equ 0x8000;; 512 Bytes After Bootloader???

ReadDisk:
	mov ah, 0x02;; set BIOS to read disk 
	mov bx, EXTENDED_SPACE
	mov al, 128;; how many sectors
	mov dl, [BOOT_DISK];; disk to read from
	mov ch, 0x00;; cylinder to read from
	mov dh, 0x00;; head to read from
	mov cl, 0x02;; sector to read (Extended_Space is 2nd sector, Bootloader is 1st)

	int 0x13;; BIOS interrupt to read disk

	jc .DiskReadFailed;; check BIOS read flag and jump to DiskReadFailed if set
	
	jnc .DiskReadPassed

	.DiskReadPassed:
		mov bx, DiskReadSuccessString
		call print_string
		jmp .ReadReturn

	.DiskReadFailed:
		mov bx, DiskReadErrorString
		call print_string
		jmp .ReadReturn

	.ReadReturn:
		ret

BOOT_DISK:
	db 0

DiskReadErrorString:
	db '...Disk Read Failed...',0

DiskReadSuccessString:
	db '...Disk Read Success...',0