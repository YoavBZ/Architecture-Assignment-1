section .text
global mulByTen, addRecursively, _add, _sub, _mul, _div
extern compareAbs, trimBignum, realloc

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





_div2:
    push rbp
    push rdi
    push rsi
    push rdx
    push rcx
    call compareAbs
    cmp al, -1
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    je .done
    push rdi
    push rcx
    push rsi
    push rdx
    mov rdi,rcx
    call mulByTwo               ; Multiplying factor by 2
    mov rdi,rsi
    call mulByTwo               ; Multiplying n2 by 2
    pop rdx
    pop rsi
    pop rcx
    pop rdi
    call _div2
    push rdi
    push rsi
    push rdx
    push rcx
    call compareAbs
    cmp al, -1
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    je .done
    push rdi
    push rsi
    push rdx
    push rcx
    mov rcx, [rsi]
    mov r8, [rdi]
    mov rdi, [rdi+8]            ; Preparing arguments for _sub
    mov rsi, [rsi+8]
    mov rdx, rdi
    call _sub
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    push rdi
    push rsi
    push rdx
    push rcx
    call trimBignum
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    push rdi
    push rsi
    push rcx
    push rdx
    mov rdi, [rdx+8]            ; Getting result.value
    mov rsi, [rdx]              ; Getting result.numOfBytes
    add rsi,[rcx]               ; Adding factor.numOfBytes
    call realloc 
    pop rdx
    pop rcx               ; Reallocating factor Bignum
    mov [rdx+8],rax
    mov r8,[rcx]
    add [rdx], r8
    push rcx
    push rdx
    mov r8, [rdx]
    mov rdi, [rdx+8]            ; Preparing arguments for _add
    mov rsi, [rcx+8]
    mov rcx, [rcx]
    mov rdx,rdi
    call _add                   ; adding factor to result
    pop rdx
    mov rdi,rdx
    push rdx
    call trimBignum
    pop rdx
    pop rcx
    pop rsi
    pop rdi

    .done:
        push rdi
        push rcx
        push rsi
        push rdx
        mov rdi,rcx
        call divByTwo               ; Dividing factor by 2
        mov rdi,rsi
        call divByTwo               ; Dividing n2 by 2
        pop rdx
        pop rsi
        pop rcx
        pop rdi

        pop rbp
        ret

_div:
    push rbp
    mov rax,[rcx+8]             ; Getting n1 value
    mov byte [rax],1
    call _div2
    push rcx
    push rdx
    push rdi
    push rsi
    call compareAbs
    cmp al, -1
    pop rsi
    pop rdi
    pop rdx
    pop rcx
    je .done
    push rdi
    push rsi
    push rdx
    push rcx
    mov rcx, [rsi]
    mov r8, [rdi]
    mov rdi, [rdi+8]            ; Preparing arguments for _sub
    mov rsi, [rsi+8]
    mov rdx, rdi
    call _sub
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    push rcx
    push rdx
    push rdi
    push rsi
    call trimBignum
    pop rsi
    pop rdi
    pop rdx
    pop rcx
    push rdi
    push rsi
    ; mov rdi, [rdx+8]            ; Getting result.value
    ; mov rsi, [rdx]              ; Getting result.numOfBytes
    ; add rsi,[rcx]               ; Adding factor.numOfBytes
    ; call realloc                ; Reallocating factor Bignum
    ; mov [rdx+8],rax
    ; mov r8, [rcx]
    ; add [rdx], r8
    push rcx
    push rdx
    mov r8, [rdx]
    mov rdi, [rdx+8]            ; Preparing arguments for _add
    mov rsi, [rcx+8]
    mov rcx, [rcx]
    mov rdx,rdi
    call _add                   ; adding factor to result
    ; pop rdx
    ; mov rdi,rdx
    ; push rdx
    ; call trimBignum
    pop rdx
    pop rcx
    pop rsi
    pop rdi

    .done:
        pop rbp
        ret

mulByTwo:
    push rbp

    mov rax, [rdi+8]                ; Getting pointer to the Bignum value
    mov rcx, [rdi]                  ; Getting numOfBytes
    mov rbx, 0
    dec rcx
    cmp rcx, 0
    jne .shifting

    .shiftOneByte:
        shl byte [rax], 1           ; Shifting the only byte
        jmp .extendBignum

    .shifting:
        shl byte [rax], 1           ; Shifting the highest order byte
        .loop:
            inc rbx
            rcl byte [rax+rbx], 1
            loop .loop, rcx         ; Moving pointer 1 byte lower

        .extendBignum:              ; Reallocate value if CF=1
            jnc .done
            push rdi
            mov rdi, [rdi+8]
            add rbx, 2
            mov rsi, rbx
            push rax
            push rbx
            push rdi
            push rcx
            call realloc
            pop rcx
            pop rdi
            pop rbx
            pop rax
            pop rdi
            mov [rdi+8], rax
            inc byte [rax+rbx-1]    ; Adding carry to the new highest order byte
            inc qword [rdi]         ; Incrementing numOfBytes
    
    .done:
        pop rbp
        ret

divByTwo:
    push rbp

    mov rax, [rdi+8]            ; Getting pointer to the Bignum value
    mov rcx, [rdi]              ; Getting numOfBytes
    dec rcx
    cmp rcx, 0
    jne .shifting

    .shiftOneByte:
        shr byte [rax], 1       ; Shifting the only byte
        jmp .done

    .shifting:
        shr byte [rax+rcx], 1   ; Shifting the highest order byte
        .loop:
            rcr byte [rax+rcx-1], 1
            loop .loop, rcx         ; Moving pointer 1 byte lower

    .done:
        push rax
        push rdi
        push rcx
        call trimBignum
        pop rcx
        pop rdi
        pop rax
        pop rbp
        ret