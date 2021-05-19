%macro IncBin 2
  SECTION .rodata
  GLOBAL %1
%1:
  incbin %2
  db 0
  %1_size: dq %1_size - %1
%endmacro

IncBin Test, "bintest.txt"

global  maxofthree
section .text
maxofthree:
        mov     rax, rdi                ; result (rax) initially holds x
        cmp     rax, rsi                ; is x less than y?
        cmovl   rax, rsi                ; if so, set result to y
        cmp     rax, rdx                ; is max(x,y) less than z?
        cmovl   rax, rdx                ; if so, set result to z
        ret 