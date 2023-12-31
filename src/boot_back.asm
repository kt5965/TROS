%include "init1.inc"
[org 0]
[bits 16]
jmp 0x07C0:start
start:
    mov ax, cs
    mov ds, ax
    ; mov ax, 0xB800
    mov es, ax
    ; mov di, 0
    ; mov ax, word [msgBack]
    ; mov cx, 0x7FF -> 점찍는 부분 삭제

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

; paint:
;     mov edi, 0
;     mov byte [es:edi], 'R'
;     inc edi
;     mov byte [es:edi], 0x20
;     inc edi
;     mov byte [es:edi], 'e'
;     inc edi
;     mov byte [es:edi], 0x20
;     inc edi
;     mov byte [es:edi], 'a'
;     inc edi
;     mov byte [es:edi], 0x20
;     inc edi
;     mov byte [es:edi], 'l'
;     inc edi
;     mov byte [es:edi], 0x20
;     inc edi
;     mov byte [es:edi], 'M'
;     inc edi
;     mov byte [es:edi], 0x20
;     inc edi
;     mov byte [es:edi], 'o'
;     inc edi
;     mov byte [es:edi], 0x20
;     inc edi
;     mov byte [es:edi], 'd'
;     inc edi
;     mov byte [es:edi], 0x20
;     inc edi
;     mov byte [es:edi], 'e'
;     inc edi
;     mov byte [es:edi], 0x20
;     inc edi

disk_read:
    mov ax, 0x1000
    mov es, ax
    mov bx, 0

    mov ah, 2
    mov dl, 0x80
    mov ch, 0
    mov dh, 0
    mov cl, 2
    mov al, 2

    int 13h

    jc error
    
    ; mov dx, 0x3F2
    ; xor al, al
    ; out dx, al

    cli
    mov al, 0x11  ; PIC를 초기화한다.
    out 0x20, al
    dw 0x00eb, 0x00eb
    out 0xA0, al
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

    msgBack db '.', 0x17

error:
    mov ax, 0xB800
    mov es, ax
    mov di, 160               ; 다음 줄의 시작 위치로 설정
    mov si, errorMsg

print_error_msg:
    ; 문자열의 끝에 도달했는지 체크
    lodsb                      ; al에 문자열의 현재 문자를 로드하고 si를 1 증가
    test al, al                ; 문자가 0인지 체크
    jz end_print               ; 0이면 출력 종료

    ; 문자를 화면에 출력
    stosw                      ; es:di에 문자를 저장하고 di를 2 증가
    add di, 2                  ; attribute byte 때문에 2 추가로 증가

    jmp print_error_msg        ; 다음 문자를 출력하기 위해 루프

end_print:

errorMsg db 'Disk Error', 0

; error2:
;     ; ah의 값을 문자로 변환
;     push ax ; ax 값을 저장

;     ; 상위 4비트를 문자로 변환
;     mov al, ah
;     shr al, 4
;     add al, '0'
;     ; al에 변환된 문자 저장됨

;     ; 문자를 화면에 출력
;     mov ah, 0x0E ; teletype mode
;     int 10h

;     ; 하위 4비트를 문자로 변환
;     pop ax
;     and ah, 0x0F
;     add ah, '0'

;     ; 문자를 화면에 출력
;     mov al, ah
;     mov ah, 0x0E ; teletype mode
;     int 10h


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