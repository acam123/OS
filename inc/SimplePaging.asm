;;page_table_entry equ 0x1000

SetupIdentityPaging:
	mov edi, [pml4]
	mov cr3, edi

	mov dword [edi], 0x2003
	add edi, [pml4]

	mov dword [edi], 0x3003
	add edi, [pml4]

	mov dword [edi], 0x4003 
	add edi, [pml4]

	mov ebx, 0x00000003
	mov ecx, 512

	.SetEntry:
		mov dword [edi], ebx
		add ebx, [pml4]
		add edi, 8
		loop .SetEntry

	mov eax, cr4
	or eax, 1 << 5       		;; Set PAE-(Physical Address Extensions)
	mov cr4, eax

	mov ecx, 0xC0000080 		;; iA32_EFER MSR selector
	rdmsr
	or eax, 1 << 8  			;; Set LME (Long Mode Enable).
	wrmsr

	mov eax, cr0
	or eax, 1 << 31				;; Set PGE-(Page Global Enable).
	mov cr0, eax

	ret