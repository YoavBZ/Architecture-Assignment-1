section .text
global mulByTen, addRecursively, _add, _sub, _mul, _div

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
        add byte [rdi], 1
        jnc .done
        loop .addCarry, rcx

    .returnCarry:
        mov al, 1
    
    .done:
        pop rbp
        ret

_add:
    push rbp
    clc                             ; Clearing CF

    .addition:    
        mov al, [rdi]
        mov byte [rdx], al          ; Adding next byte of n1
        mov bl, [rsi]
        adc byte [rdx], bl          ; Substracting next byte of n2
        inc rdi                     ; Continuing to the next byte
        inc rsi                     ; Continuing to the next byte
        inc rdx                     ; Continuing to the next byte
        dec r8
        loop .addition, rcx
        jnc .append
        
    .hasCarry:
        cmp r8, 0
        je .lastByte
        mov al, [rdi]
        mov byte [rdx], al          ; Adding next byte of n1
        add byte [rdx], 1           ; Handle carry
        inc rdi
        inc rdx
        dec r8
        jc .hasCarry
    
    .append:
        cmp r8, 0
        je .done                    ; Equal sizes, done if CF = 0
        mov al, [rdi]
        mov byte [rdx], al          ; Adding next byte of n1
        inc rdi                     ; Continuing to the next byte
        inc rdx                     ; Continuing to the next byte
        dec r8
        jmp .append
    
    .lastByte:
        add byte [rdx], 1

    .done:
        pop rbp
        ret

_sub:
    push rbp
    clc                             ; Clearing CF

    .substract:    
        mov al, [rdi]
        mov byte [rdx], al          ; Adding next byte of n1
        mov bl, [rsi]
        sbb byte [rdx], bl          ; Substracting next byte of n2
        inc rdi                     ; Continuing to the next byte
        inc rsi                     ; Continuing to the next byte
        inc rdx                     ; Continuing to the next byte
        dec r8
        loop .substract, rcx
        jnc .append
    
    .hasCarry:
        mov al, [rdi]
        mov byte [rdx], al          ; Adding next byte of n1
        sub byte [rdx], 1           ; Handle carry
        inc rdi
        inc rdx
        dec r8
        jc .hasCarry
    
    .append:
        cmp r8, 0
        je .done                    ; Equal sizes, done if CF = 0
        mov al, [rdi]
        mov byte [rdx], al          ; Adding next byte of n1
        inc rdi                     ; Continuing to the next byte
        inc rdx                     ; Continuing to the next byte
        dec r8
        jmp .append
    
    .done:
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