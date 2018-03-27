section .text
global mulByTen, addRecursively

mulByTen:
    push rbp

    mov rax, rdi                ; Get cell
    mov rbx, 0xA
    mul rbx
    mov [rdi], rax
    mov rax, rdx                ; Save higher order qword

    pop rbp                     ; Restore caller state
    ret

addRecursively:
    push rbp
    
    add qword [rdi], rsi        ; Adding the value to the currect address
    mov rcx, rdx
    addCarry:
        mov rdi, rdi+8
        adc qword [rdi], 0
        loop addCarry, rdx
    
    pop rbp
    ret