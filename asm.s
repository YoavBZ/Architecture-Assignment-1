section .text
global mulByTen, addRecursively, _add, _sub, _mul, _div

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
    jnc .done
    
    cmp rdx, 0
    je done
    mov rcx, rdx
    addCarry:
        inc rdi
        adc byte [rdi], 0
        loop addCarry, rcx
    jnc .done
    mov ax, 1
    
    .done:
        pop rbp
        ret

_add:
    push rbp
    
    mov al, [rdi]
    mov byte [rdx], al              ; Adding lowest order word of n1
    mov bl, [rsi]
    add byte [rdx], bl              ; Adding lowest order word of n2
    dec rcx
    cmp rcx, 0
    je .done

    .next:
        inc rdi
        inc rsi
        inc rdx
        mov al, [rdi]
        mov byte [rdx], al          ; Adding next word of n1
        mov bl, [rsi]
        adc byte [rsi], bl          ; Adding next word of n2
        loop .next, rcx
    
    .done:
        adc byte [rdx+1], 0              ; Add carry to highest order word if exists
        pop rbp
        ret

_sub:
    push rbp
    
    mov al, [rdi]
    mov byte [rdx], al              ; Adding lowest order word of n1
    mov bl, [rsi]
    add byte [rdx], bl              ; Adding lowest order word of n2
    dec rcx
    cmp rcx, 0
    je .done

    .next:
        inc rdi
        inc rsi
        inc rdx
        mov al, [rdi]
        mov byte [rdx], al          ; Adding next word of n1
        mov bl, [rsi]
        adc byte [rsi], bl          ; Adding next word of n2
        loop .next, rcx
    
    .done:
        adc byte [rdx+1], 0              ; Add carry to highest order word if exists
        pop rbp
        ret

_mul:
    push rbp
    
    mov al, [rdi]
    mov byte [rdx], al              ; Adding lowest order word of n1
    mov bl, [rsi]
    add byte [rdx], bl              ; Adding lowest order word of n2
    dec rcx
    cmp rcx, 0
    je .done

    .next:
        inc rdi
        inc rsi
        inc rdx
        mov al, [rdi]
        mov byte [rdx], al          ; Adding next word of n1
        mov bl, [rsi]
        adc byte [rsi], bl          ; Adding next word of n2
        loop .next, rcx
    
    .done:
        adc byte [rdx+1], 0              ; Add carry to highest order word if exists
        pop rbp
        ret

_div:
    push rbp
    
    mov al, [rdi]
    mov byte [rdx], al              ; Adding lowest order word of n1
    mov bl, [rsi]
    add byte [rdx], bl              ; Adding lowest order word of n2
    dec rcx
    cmp rcx, 0
    je .done

    .next:
        inc rdi
        inc rsi
        inc rdx
        mov al, [rdi]
        mov byte [rdx], al          ; Adding next word of n1
        mov bl, [rsi]
        adc byte [rsi], bl          ; Adding next word of n2
        loop .next, rcx
    
    .done:
        adc byte [rdx+1], 0              ; Add carry to highest order word if exists
        pop rbp
        ret