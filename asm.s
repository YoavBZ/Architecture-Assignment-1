section .text
global mulByTen
global addRecursively

mulByTen:
    push rbp

    mov al, [rdi]                ; Get cell
    mov bl, 10
    mul bl
    mov [rdi], al              ; Save lower order qword
    mov al, ah                ; Save higher order qword

    pop rbp                     ; Restore caller state
    ret

addRecursively:
    push rbp
    
    mov ax, 0                ; Resetting the return value
    add word [rdi], si       ; Adding the value to the currect address
    jnc done
    
    cmp rdx, 0
    je done
    mov rcx, rdx
    addCarry:
        adc word [rdi+1], 0
        inc rdi
        loop addCarry, rcx
    jnc done
    mov ax, 1
    
    done:
        pop rbp
        ret