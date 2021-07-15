ActivateSSE:				;; support floats
	mov rax, cr0
	and ax, 0b11111101		;; set cr0 bit_1 -> 0 
	or ax, 0b00000001		;; set cr0 bit_0 -> 1
	mov cr0, rax

	mov rax, cr4
	or ax, 0b1100000000		;; set cr4 bit_8 & bit_9 -> 1
	mov cr4, rax
	ret