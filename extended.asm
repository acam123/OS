;;[org 0x8000] ;; moved to linker
[bits 16]

mov bx, ExtendedString
call PrintString

jmp EnterProtectedMode

%include "inc/GlobalVars.asm"
%include "inc/PrintString.asm"
%include "inc/Gdt.asm"
%include "inc/DetectMemory.asm"



EnterProtectedMode:
	call DetectMemory
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

	call DetectCPUID
	call DetectLongMode
	call SetupIdentityPaging
	call EditGDT

	jmp codeseg:Start64BitMode

[bits 64]
[extern _start]
%include "inc/IDT.asm"
%include "inc/LoadBins.asm"

Start64BitMode:

	call _start

	jmp $
	

ExtendedString:
	db '...This is Extended Space...',0

times 2048-($-$$) db 0


