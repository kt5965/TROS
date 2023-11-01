%include "init1.inc"

[org 0x10000]
[bits 32]

PM_start:
    mov bx, SysDataSelector
    mov ds, bx
    mov es, bx

    mov ax, VideoSelector 
    mov es, ax

    mov edi, 80*2*10+2*10  
    call printf            

    jmp $

printf:

    mov byte [es:edi], 'p'
    inc edi
    mov byte [es:edi], 0x47
    inc edi
    mov word [es:edi], 'r'
    inc edi
    mov byte [es:edi], 0x47
    inc edi
    mov word [es:edi], 'o'
    inc edi
    mov byte [es:edi], 0x47
    inc edi
    mov word [es:edi], 't'
    inc edi
    mov byte [es:edi], 0x47
    inc edi
    mov word [es:edi], 'e'
    inc edi
    mov byte [es:edi], 0x47
    inc edi
    mov word [es:edi], 'c'
    inc edi
    mov byte [es:edi], 0x47
    inc edi
    mov word [es:edi], 't'
    inc edi
    mov byte [es:edi], 0x47
    inc edi
    mov word [es:edi], 'e'
    inc edi
    mov byte [es:edi], 0x47
    inc edi
    mov word [es:edi], 'd'
    inc edi
    mov byte [es:edi], 0x47
    inc edi
    mov word [es:edi], ' '
    inc edi
    mov byte [es:edi], 0x47
    inc edi
    mov word [es:edi], 'M'
    inc edi
    mov byte [es:edi], 0x47
    inc edi
    mov word [es:edi], 'o'
    inc edi
    mov byte [es:edi], 0x47
    inc edi
    mov word [es:edi], 'd'
    inc edi
    mov byte [es:edi], 0x47
    inc edi
    mov word [es:edi], 'e'
    inc edi
    mov byte [es:edi], 0x47
    inc edi

    ret
