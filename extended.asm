;;[org 0x8000] ;; moved to linker
[bits 16]

mov bx, ExtendedString
call PrintString

jmp EnterProtectedMode

%include "inc/PrintString.asm"
%include "inc/Gdt.asm"


EnterProtectedMode:
	call EnableA20

	cli ;;disable interrupts
	lgdt [gdt_descriptor]
	mov eax, cr0
	or eax, 1
	mov cr0, eax
	jmp codeseg:StartProtectedMode

EnableA20:
	in al, 0x92
	or al, 2
	out 0x92, al
	ret
	
[bits 32]

%include "inc/CPUID_and_LongMode.asm"
%include "inc/SimplePaging.asm"

StartProtectedMode: 

	mov ax, dataseg
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ebp, 0x90000
    mov esp, ebp

	;;mov [0xb8000], byte '!' ;; overwrite BIOS video buffer
	;;mov [0xb8001], byte 0x0c 
	;;mov [0xb8002], byte '!' 
	;;mov [0xb8003], byte 0x09 
	;;mov [0xb8004], byte '!' 
	;;mov [0xb8005], byte 0x0a 

	call DetectCPUID
	call DetectLongMode
	call SetupIdentityPaging
	call EditGDT

	jmp codeseg:Start64BitMode

[bits 64]
[extern _start]


Start64BitMode:

	;;mov edi, 0xb8000
	;;mov rax, 0x0000000001f001f00
	;;mov ecx, 1
	;;rep stosq


	call _start

	jmp $
	

ExtendedString:
	db '...This is Extended Space...',0

times 2048-($-$$) db 0
