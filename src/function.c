#include "data.h"
#include "function.h"

void kprintf(char* str, int line, int col)
{
    char *video = (char*)(0xB8000 + 160 * line + col*2);
	for (int i = 0; str[i] != 0; i++)
	{
		*video++ = str[i];
		*video++ = 0x03; 
	}
    return;
}

// ������ itoa �Լ�. ���ڸ� ���ڿ��� ��ȯ
char* itoa(int value, char* str, int base) {
    char* ptr = str, *ptr1 = str;
    char tmp_char;
    int tmp_value;

    // üũ: �κ����� ���̽�
    if (base < 2 || base > 36) { *str = '\0'; return str; }

    do {
        tmp_value = value; // ���� value�� 10 �̸�
        value /= base; // base �� ���� ���� value���ϰ�
        *ptr++ = "0123456789abcdefghijklmnopqrstuvwxyz"[tmp_value - value * base];
    } while ( value );

    // ������ ��� '-' ��ȣ �߰�
    if (tmp_value < 0) 
        *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return str;
}

// // ȭ�鿡 ���ڸ� ���
// void kprintf_at(const char* message, int col, int row) {
//     // VGA �޸� ���� �ּ�
//     volatile char* video_memory = (volatile char*)0xb8000;
//     int offset = (row * 80 + col) * 2; // 2 ����Ʈ �� ���� �ϳ�

//     // �޽��� ���
//     while (*message) {
//         video_memory[offset] = *message++;
//         video_memory[offset + 1] = 0x07;
//         offset += 2;
//     }
// }

void kprintf_line_clear(int line, int col)
{
    volatile char* video_memory = (volatile char*)(0xB8000 + 160*line + 2*col);
    for (int i = 0; i < 160-2*col; i++)
    {
        *video_memory++ = 0;
        *video_memory++ = 0x7;
    }
}

void kprintf_clear_all()
{
    for (int i = 0; i < VIDEOMAXLINE; i++)
    {
        kprintf_line_clear(i, 0);
    }
}

int kstrcmp(char* str1, char* str2)
{
    for (int i = 0; i < kstrlen(str1); i++)
    {
        if (str1[i] != str2[i])
            return FALSE;        
    }
    return TRUE;
}

int kstrlen(char* str1)
{
    int i = 0;
    while(str1[i] != '\0')
    {
        i++;
    }
    return i;
}

// unsigned char HDDstatus()
// {
// 	unsigned char value;
// 	__asm__ __volatile__
// 	(
// 		"mov dx,0x1F7;"
// 		"in al, dx;"
// 	);
// 	__asm__ __volatile__("mov %0, al;" :"=r"(value));

// 	return value;

// }

// int HDD_BUSY() // 7�� ��Ʈ Ȯ���ؼ� busy Ȯ��
// {
//     unsigned char status = HDDstatus();
//     if ((status & 0x80) == 0x80)
//     {
//         return 1;
//     }
//     else
//     {
//         return 0;
//     }
// }

// int HDD_READY() // 6����Ʈ �غ� Ȯ�� 
// {
// 	unsigned char status = HDDstatus();

//     if ((status&0x40) == 0x40)
//     {
//         return 1;
//     }
//     else
//     {
//         return 0;
//     }
// }

// int HDD_DRQ() // �����Ͱ� �� ���� �غ� �Ǿ����� Ȯ�� 3����Ʈ
// {
// 	unsigned char status = HDDstatus();
//     if ((status&0x08) == 0x08)
//     {
//         return 1;
//     }
//     else
//     {
//         return 0;
//     }
// }

// int HDD_ERR() // ERR Ȯ�� 0�� ��Ʈ
// {
// 	unsigned char status = HDDstatus();

//     if ((status&0x01) == 0x01)
//     {
//         return 1;
//     }
//     else
//     {
//         return 0;
//     }
// }

// void HDDread(unsigned int sector, char* buffer)
// {
//     // LBA �ּҷ� ����
//     unsigned char LBA_a = sector & 0xFF;
//     unsigned char LBA_b = (sector >> 8) & 0xFF;
//     unsigned char LBA_c = (sector >> 16) & 0xFF;
//     // 0xE0 -> LBA ��带 Ű�� ���� ���� ��Ʈ ����
//     unsigned char LBA_d = ((sector >> 24) & 0xFF) | 0xE0;;

//     __asm__ __volatile__
//     (
//         // 0x3F6 ��Ʈ�� 0 ���� ���� �ϵ��ũ ��Ʈ�ѷ� ����
//         // dx �������ʹ� IO ��Ʈ �ּҸ� �����ϴµ� ���
//         // 0x1F0: ������ ��������, ������ �б�/���⿡ ���.
//         // 0x1F1: ���� ��������, ���� �ڵ� �б⿡ ���.
//         // 0x1F2: ���� ī��Ʈ ��������, �аų� �� ������ �� ����.
//         // 0x1F3-0x1F5: LBA �ο�/�̵�/���� ��������, LBA �ּ� ����.
//         // 0x1F6: ����̺�/��� ��������, LBA ��� �� ����̺� ����.
//         // 0x1F7: Ŀ�ǵ� ��������, 
//         "mov al, 0;"
// 		"mov dx, 0x3F6;"
// 		"out dx, al;"
//         "nop;"
//     );

// 	__asm__ __volatile__
// 	(
//         // 0x1F2�� ��Ʈ�� ���� ���� ���� ����
// 		"mov al, 0x01;"
// 		"mov dx, 0x1F2;"
// 		"out dx, al;"
//         "nop;"
// 	);
//     __asm__ __volatile__
// 	(
//         // 01F6���� 01F5���� ��Ʈ�� LBA �ּ��� �� �κ��� ������
//         // al �������� �����͸� �����ϰų� ����� �����µ� ���
// 		"mov al, %0;"
// 		"mov dx, 0x1F6;"
// 		"out dx, al;"::"r"(LBA_d)
// 	);



// 	__asm__ __volatile__
// 	(
// 		"mov al, %0;"
// 		"mov dx, 0x1F3;"
// 		"out dx, al;" ::"r"(LBA_a)
// 	);

// 	__asm__ __volatile__
// 	(
// 		"mov al, %0;"
// 		"mov dx, 0x1F4;"
// 		"out dx, al;" ::"r"(LBA_b)
// 	);

// 	__asm__ __volatile__
// 	(
// 		"mov al, %0;"
// 		"mov dx, 0x1F5;"
// 		"out dx, al;" ::"r"(LBA_c)
// 	);


// 	__asm__ __volatile__
// 	(
//         // 0x1F7�� ��Ʈ�� �б��� 0x20�� ����
// 		"mov al, 0x20;"
// 		"mov dx,0x1F7;"
// 		"out dx, al;"
// 	);

//     __asm__ __volatile__
//     (
//         // 0x1F0�� ��Ʈ���� �����͸� �о�� ���ۿ� ����
//         "mov dx,0x1F0;"
//         // edi : �����͸� ������ �޸� �ּ�
//         "mov edi, %0;"
//         // �ݺ���ɿ��� ����� ī���͸� ����
//         "mov ecx, 256;"
//         "rep insw;" // 16��Ʈ ������ �����͸� �о�� edi�� ����Ű�� �޸� ��ġ�� ����
//         :: "r"(buffer)
//     );
// }

