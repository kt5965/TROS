%include "init1.inc"
[org 0]
[bits 16]
jmp 0x07C0:start
start:
    mov ax, cs
    mov ds, ax
    mov es, ax

reset:
    mov ax, 0
    mov dl, 0x80
    int 13h
    jc reset

    mov ax, 0xB800
    mov es, ax
    mov di, 0
    mov ax, word [msgBack]
    mov cx, 0x7FF
paint:
    mov word [es:di], ax
    add di, 2
    dec cx
    jnz paint

disk_read:
    mov ax, 0x1000
    mov es, ax
    mov bx, 0

    mov ah, 2
    mov dl, 0x80
    mov ch, 0
    mov dh, 0
    mov cl, 2
    mov al, 10

    int 13h

    jc error
    
    cli
    mov al, 0x11  ; PICλ₯? μ΄κΈ°???€.
    out 0x20, al  ; Master PIC
    dw 0x00eb, 0x00eb ; jmp $+2, jmp $+2
    out 0xA0, al  ; Slave PIC
    dw 0x00eb, 0x00eb

    mov al, 0x20
    out 0x21, al
    dw 0x00eb, 0x00eb
    mov al, 0x28
    out 0xA1, al
    dw 0x00eb, 0x00eb

    mov al, 0x04
    out 0x21, al
    dw 0x00eb, 0x00eb
    mov al, 0x02
    out 0xA1, al
    dw 0x00eb, 0x00eb

    mov al, 0x01
    out 0x21, al
    dw 0x00eb, 0x00eb
    out 0xa1, al
    dw 0x00eb, 0x00eb

    mov al, 0xFF
    out 0xA1, al
    dw 0x00eb, 0x00eb
    mov al, 0xFB
    out 0x21, al


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
mov fs, bx
mov gs, bx
mov ss, bx

jmp dword SysCodeSelector:0x10000

msgBack db ' ', 0x17

error:
    mov ax, 0xB800
    mov es, ax
    mov di, 160               ; ?€? μ€μ ?? ?μΉλ‘ ?€? 
    mov si, errorMsg

print_error_msg:
    ; λ¬Έμ?΄? ?? ??¬??μ§? μ²΄ν¬
    lodsb                      ; al? λ¬Έμ?΄? ??¬ λ¬Έμλ₯? λ‘λ?κ³? siλ₯? 1 μ¦κ??
    test al, al                ; λ¬Έμκ°? 0?Έμ§? μ²΄ν¬
    jz end_print               ; 0?΄λ©? μΆλ ₯ μ’λ£

    ; λ¬Έμλ₯? ?λ©΄μ μΆλ ₯
    stosw                      ; es:di? λ¬Έμλ₯? ????₯?κ³? diλ₯? 2 μ¦κ??
    add di, 2                  ; attribute byte ?λ¬Έμ 2 μΆκ??λ‘? μ¦κ??

    jmp print_error_msg        ; ?€? λ¬Έμλ₯? μΆλ ₯?κΈ? ??΄ λ£¨ν

end_print:

errorMsg db 'Disk Error', 0


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