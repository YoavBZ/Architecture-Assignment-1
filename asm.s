section .text
global mulByTen, addRecursively, _add

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
        inc rdi
        adc word [rdi], 0
        loop addCarry, rcx
    jnc done
    mov ax, 1
    
    done:
        pop rbp
        ret

_add:
    push rbp
    
    mov word [rdx], [rdi]              ; Storing lowest order word of n1
    add word [rdx], [rsi]              ; Adding lowest order word of n2
    dec rcx
    cmp rcx, 0
    je done

    next:
        inc rdi
        inc rsi
        inc rdx
        mov word [rdx], [rdi]
        adc word [rsi], [rsi] 
        loop next, rcx
    
    done:
        adc [rdx+1], 0
        pop rbp
        ret