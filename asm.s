section .text
global _sum

_sum:
    push rbp                    ; Save caller state
    mov rbp, rsp

    mov rax, rdi        ; Copy function args to registers: leftmost...
    mov rbx, rsi        ; Next argument...
    add rax,rbx                 ; sum 2 arguments
    mov [rbp-8], rax

    pop rbp                     ; Restore caller state
    ret