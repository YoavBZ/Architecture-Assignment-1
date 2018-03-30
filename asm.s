section .text
global mulByTen
global addRecursively
global _add
global _sub
global _mul
global _div

mulByTen:
    push rbp

    mov al, [rdi]                ; Get cell
    mov bl, 10
    mul bl
    mov [rdi], al              ; Save lower order byte
    mov al, ah                ; Save higher order byte

    pop rbp                     ; Restore caller state
    ret

addRecursively:
    push rbp
    
    mov al, 0                ; Resetting the return value
    mov bx ,si
    add byte [rdi], bl       ; Adding the value to the currect address
    jnc .done
    
    cmp rdx, 0
    je .returnCarry
    mov rcx, rdx
    .addCarry:
        inc rdi
        adc byte [rdi], 0
        jnc .done
        loop .addCarry, rcx
    jnc .done

    .returnCarry:
        mov al, 1
    
    .done:
        pop rbp
        ret

_add:
    push rbp
    
    mov al, [rdi]
    mov byte [rdx], al              ; Adding lowest order byte of n1
    mov bl, [rsi]
    add byte [rdx], bl              ; Adding lowest order byte of n2
    dec rcx                         ; Decrementing the number of bytes left in n1
    dec r8                          ; Decrementing the number of bytes left in n2
    cmp r8, 0                       ; Checking if finished n1
    je .done

    .next:
        inc rdi                     ; Continuing to the next byte
        inc rsi                     ; Continuing to the next byte
        inc rdx                     ; Continuing to the next byte
        mov al, [rdi]
        adc byte [rdx], al          ; Adding next byte of n1
        dec r8
        cmp r8, 0                   ; Checking if finished n1
        je .done
        cmp rcx, 0                  ; Checking if finished n2
        je .next
        mov bl, [rsi]
        adc byte [rdx], bl          ; Adding next byte of n2
        loop .next, rcx    

    .done:
        adc byte [rdx+1], 0
        pop rbp
        ret

_sub:
    push rbp
    
    mov al, [rdi]
    mov byte [rdx], al              ; Adding lowest order byte of n1
    mov bl, [rsi]
    sub byte [rdx], bl              ; Adding lowest order byte of n2
    dec rcx                         ; Decrementing the number of bytes left in n1
    dec r8                          ; Decrementing the number of bytes left in n2
    cmp r8, 0                       ; Checking if finished n1
    je .done

    .next:
        inc rdi                     ; Continuing to the next byte
        inc rsi                     ; Continuing to the next byte
        inc rdx                     ; Continuing to the next byte
        mov al, [rdi]
        mov byte [rdx], al          ; Adding next byte of n1
        dec r8
        cmp r8, 0                   ; Checking if finished n1
        je .done
        cmp rcx, 0                  ; Checking if finished n2
        je .next
        mov bl, [rsi]
        sbb byte [rdx], bl          ; Adding next byte of n2
        loop .next, rcx    

    .done:
        sbb byte [rdx+1], 0
        pop rbp
        ret

_mul:
    ; push rbp
    
    ; mov al, [rdi]
    ; mov byte [rdx], al              ; Adding lowest order byte of n1
    ; mov bl, [rsi]
    ; add byte [rdx], bl              ; Adding lowest order byte of n2
    ; dec rcx
    ; cmp rcx, 0
    ; je .done

    ; .next:
    ;     inc rdi
    ;     inc rsi
    ;     inc rdx
    ;     mov al, [rdi]
    ;     mov byte [rdx], al          ; Adding next byte of n1
    ;     mov bl, [rsi]
    ;     adc byte [rsi], bl          ; Adding next byte of n2
    ;     loop .next, rcx
    
    ; .done:
    ;     adc byte [rdx+1], 0              ; Add carry to highest order byte if exists
    ;     pop rbp
    ;     ret

_div:
    ; push rbp
    
    ; mov al, [rdi]
    ; mov byte [rdx], al              ; Adding lowest order byte of n1
    ; mov bl, [rsi]
    ; add byte [rdx], bl              ; Adding lowest order byte of n2
    ; dec rcx
    ; cmp rcx, 0
    ; je .done

    ; .next:
    ;     inc rdi
    ;     inc rsi
    ;     inc rdx
    ;     mov al, [rdi]
    ;     mov byte [rdx], al          ; Adding next byte of n1
    ;     mov bl, [rsi]
    ;     adc byte [rsi], bl          ; Adding next byte of n2
    ;     loop .next, rcx
    
    ; .done:
    ;     adc byte [rdx+1], 0              ; Add carry to highest order byte if exists
    ;     pop rbp
    ;     ret