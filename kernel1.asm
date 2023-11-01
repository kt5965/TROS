%include "init1.inc"

[org 0x10000]
[bits 32]

PM_start:
    mov bx, SysDataSelector
    mov ds, bx
    mov es, bx

    mov ax, VideoSelector 
    mov es, ax

    mov edi, 0
    call printf            

    jmp $

printf:

    mov byte [es:edi], 'p'
    inc edi
    mov byte [es:edi], 0x47
    inc edi
    mov word [es:edi], 'r'
    inc edi
    mov byte [es:edi], 'p'
    inc edi
    mov byte [es:edi], 0x47
    inc edi
    mov word [es:edi], 'r'
    inc edi
    mov byte [es:edi], 'p'
    inc edi
    mov byte [es:edi], 0x47
    inc edi
    mov word [es:edi], 'r'
    inc edi
    mov byte [es:edi], 'p'
    inc edi
    mov byte [es:edi], 0x47
    inc edi
    mov word [es:edi], 'r'
    inc edi


    ret
