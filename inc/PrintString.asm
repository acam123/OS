print_string:
        mov ah, 0x0e
        mov al, 10;; new line
        int 0x10

        mov ah, 0x0e
        mov al, 13;; carriage return
        int 0x10

        mov ah, 0x0e;; start a BIOS print interrupt routine
        .Loop:
        cmp [bx], byte 0
        je .Exit
                mov al, [bx];; move string character to print register
                int 0x10;; perform BIOS interrupt to print the character
                inc bx
                jmp .Loop
        .Exit:
        ret