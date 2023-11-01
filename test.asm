%include "init1.inc"

[org 0x10000]
[bits 32]

PM_Start:
    mov bx, SysDataSelector
    mov ds, bx
    mov es, bx
    mov ax, VideoSelector
    mov es, ax

    mov edi, 80*2*10+2*10
    call printf

    jmp $

printf:
    mov edi, 0
    mov byte [es:edi], 'P'
    inc edi
    mov byte [es:edi], '0x17'
    inc edi
    mov byte [es:edi], 'F'

    ret