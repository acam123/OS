PrintHex:

	;;First Nibble
	mov dh, bh
	shr dh, 4;; 

	add dh, 0x30
	cmp dh, 0x39

	jle nib_one
	add dh, 0x27

	nib_one:
		mov [HEX_OUT+2], byte dh

	;; Second Nibble
	mov dl, bh
	and dl, 0x0f;;

	add dl, 0x30
	cmp dl, 0x39

	jle nib_two
	add dl, 0x27

	nib_two:
		mov [HEX_OUT+3], byte dl

	;; Third Nibble
	mov dh, bl
	shr dh, 4

	add dh, 0x30
	cmp dh, 0x39

	jle nib_three
	add dh, 0x27

	nib_three:
		mov [HEX_OUT+4], byte dh

	;; Fourth Nibble
	mov dl, bl
	and dl, 0x0f;;

	add dl, 0x30
	cmp dl, 0x39

	jle nib_four
	add dl, 0x27

	nib_four:
		mov [HEX_OUT+5], byte dl

	;; Print Hex
	mov bx, HEX_OUT
	call PrintString
ret

HEX_OUT:
	db '0x----',0 

ASCII_TABLE:
	db '0123456789abcdef',0