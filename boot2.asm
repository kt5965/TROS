%include "init1.inc"
[org 0]     
jmp 0x07C0:start        
start:                      
    mov ax, cs
    mov ds, ax
    mov ax, 0xB800
    mov es, ax
    mov di, 0
    mov cx, 0x7FF

paint:
    mov edi, 0
    mov byte [es:edi], 'T'
    inc edi
    mov byte [es:edi], 0x05
    inc edi
    mov byte [es:edi], 'A'
    inc edi
    mov byte [es:edi], 0x16
    inc edi
    mov byte [es:edi], 'E'
    inc edi
    mov byte [es:edi], 0x27
    inc edi
    mov byte [es:edi], 'R'
    inc edi
    mov byte [es:edi], 0x30
    inc edi
    mov byte [es:edi], 'A'
    inc edi
    mov byte [es:edi], 0x41
    inc edi
    mov byte [es:edi], 'N'
    inc edi
    mov byte [es:edi], 0x41
    inc edi
    mov byte [es:edi], 'G'
    inc edi


disk_read:
    mov ax, 0x1000
    mov es, ax
    mov bx, 0

    mov ah, 2
    mov dl, 0
    mov ch, 0
    mov dh, 0
    mov cl, 2
    mov al, 1


    int 13h

    jc disk_read
    
    cli

    lgdt[gdtr]

    mov eax, cr0
    or eax, 0x00000001
    mov cr0, eax

    jmp $+2
    nop
    nop

    mov bx, SysDataSelector
    mov ds, bx
    mov es, bx
    mov ss, bx

    jmp dword SysCodeSelector:0x10000


gdtr:
    dw gdt_end - gdt - 1
    dd gdt+0x7C00 

gdt:
    dd 0x00000000, 0x00000000 
    dd 0x0000FFFF, 0x00CF9A00
    dd 0x0000FFFF, 0x00CF9200
    dd 0x8000FFFF, 0x0040920B

gdt_end:
    times 510-($-$$) db 0
    dw 0xAA55