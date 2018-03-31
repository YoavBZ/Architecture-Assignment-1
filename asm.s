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
    push rbp
    
    mov r9, 0                            ; Init outerLoop index
    mov r10, 0                           ; Init innerLoop index
    mov r13, rsi

    outerLoop:
        mov al, [rdi]
        mov r11, rdx                     ; Saving result array
        innerLoop:
            mov bl, [rsi]
            mul bl                       ; Adding lowest order byte of n2
            push rsi
            push rbx
            push rcx
            push rdi
            push rdx
            push rax
            mov rdi, r11
            mov sil, al
            mov rdx, r8                 ; Sending the number of cells left in the array
            add rdx, rcx                ; -"-
            sub rdx, r9                 ; -"-
            sub rdx, r10                ; -"-
            dec rdx                     ; -"-
            mov r12, rdx
            call addRecursively         ; Adding the lower order carry to the higher order bytes
            mov rdi, r11
            inc rdi
            pop rax
            mov al, ah
            mov sil, al                 ; Sending the multiplication carry
            mov rdx, r12                ; Sending the number of cells left in the array
            dec rdx
            call addRecursively         ; Adding the multiplication carry to the higher order bytes
            pop rdx
            pop rdi
            pop rcx
            pop rbx
            pop rsi
            inc rsi
            inc r10                     ; Incrementing innerLoop index
            inc r11                     ; Moving to the next result cell
            mov al, [rdi]
            cmp r10, r8
            jne innerLoop
            mov r10, 0
            mov rsi, r13                ; Restoring the currect n2 cell
            inc rdi
            inc rdx                     ; Restoring the currect result cell
            inc r9
            cmp r9, rcx
            jne outerLoop
    
    pop rbp
    ret

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