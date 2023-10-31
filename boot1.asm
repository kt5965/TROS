[org 0]                     ; 메모리의 어디에서 시작할지 지정
[bits 16]                   ; 16비트로 동작할 것이다.
    jmp 0x07C0:start        ; 이걸 기계어로 번역하면 EA0500C007이 된다. EA는 jmp, 05는 start C007은 07C0이다. 리틀 엔디안을 따른다.

start:                      
    mov ax, cs
    mov ds, ax
    mov ax, 0xB800
    mov es, ax
    mov di, 0
    mov ax, word [msgBack]
    mov cx, 0x7EF

paint:
    mov word [es:di], ax
    add di, 2
    dec cx
    jnz paint

    mov di, 0
    mov byte [es:di], 'T'
    inc di
    mov byte [es:di], 0x05
    inc di
    mov byte [es:di], 'A'
    inc di
    mov byte [es:di], 0x16
    inc di
    mov byte [es:di], 'E'
    inc di
    mov byte [es:di], 0x27
    inc di
    mov byte [es:di], 'R'
    inc di
    mov byte [es:di], 0x30
    inc di
    mov byte [es:di], 'A'
    inc di
    mov byte [es:di], 0x41
    inc di
    mov byte [es:di], 'N'
    inc di
    mov byte [es:di], 0x41
    inc di
    mov byte [es:di], 'G'

    jmp $

msgBack db '.', 0x17

times 510-($-$$) db 0

dw 0xAA55