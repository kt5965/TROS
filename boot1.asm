[org 0]                     ; �޸��� ��𿡼� �������� ����
[bits 16]                   ; 16��Ʈ�� ������ ���̴�.
    jmp 0x07C0:start        ; �̰� ����� �����ϸ� EA0500C007�� �ȴ�. EA�� jmp, 05�� start C007�� 07C0�̴�. ��Ʋ ������� ������.

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