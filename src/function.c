#include <stdarg.h>
#include "data.h"
#include "function.h"

void kprintf(char* str, int line, int col, ...)
{
    va_list va;
    va_start(va,col);

    char *video = (char*)(0xB8000 + 160 * line + col*2);
	for (int i = 0; str[i] != 0; i++)
	{
		if ( str[i] == '%' && str[i+1] == 'd')
		{
			kprintf_arg(str+i, ((int)va_arg(va,int)), &video);
			i++;
		}
		else
		{
			*video++ = str[i];
			*video++ = 0x03; 
		}
	}
    va_end(va);

    return;
}

void printf(char* str, ...)
{
    va_list va;
    va_start(va, str);

    char *video = (char*)(0xB8000 + 160 * ++curline + 0);
	for (int i = 0; str[i] != '\0'; i++)
	{
		if (str[i] == '%' && (str[i+1] == 'd' || str[i+1] == 's'))
		{
			kprintf_arg(str+i, (int)va_arg(va,int), &video);
			i++;
		}
		else
		{
			*video++ = str[i];
			*video++ = 0x03; 
		}
	}
    va_end(va);

    return;
}


char* kstrcpy(char* dest, const char* src) {
    char* original_dest = dest;
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return original_dest;
}
int kmemcpy(char* dest, char* src, int size) {
    int i;
    for (i = 0; i < size; i++)
        dest[i] = src[i];
    return i;
}

int kmemset(char* dest, int value, int size) {
	int i;
	for (i = 0; i < size; i++)
		dest[i] = value;
	return i;
}


void kprintf_arg(const char* format, int target, char** video)
{
	if (format[1] == 'd') {
		unsigned char buffer[64];
		int i = 0;
		int isNegative = 0;

		if ((int)target < 0) {
			isNegative = 1;
			target = -(int)target;
			buffer[i++] = '-';
		}

		int start = i;
		do {
			buffer[i++] = '0' + (target % 10);
			target /= 10;
		} while (target > 0);

		if (isNegative) {
			buffer[i++] = '-';
		}
		buffer[i] = '\0';
        for (int j = i - 1; j >= start; j--) {
            **video = buffer[j];
            (*video)++;
            **video = 0x03;
            (*video)++;
        }
	}
	else if (format[1] == 's') {
        char* str = (char*)target;
        while (*str != '\0') {
            **video = *str++;
            (*video)++;
            **video = 0x03;
            (*video)++;
        }
    }
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


// ȭ�鿡 ���ڸ� ���
void kprintf_at(const char* message, int col, int row) {
    // VGA �޸� ���� �ּ�
    volatile char* video_memory = (volatile char*)0xb8000;
    int offset = (row * 80 + col) * 2; // 2 ����Ʈ �� ���� �ϳ�

    // �޽��� ���
    while (*message) {
        video_memory[offset] = *message++;
        video_memory[offset + 1] = 0x07;
        offset += 2;
    }
}

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



int kstrcmp(char* str1, char*str2)
{
    for (int i = 0; i < kstrlen(str2); i++)
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

unsigned char HDDstatus()
{
    unsigned char value;

    __asm__ __volatile__
    (
        "mov dx, 0x1F7;"
        "in al, dx;"
        "mov %0, al;"
        :"=r"(value)
    );

    return value;
}

int HDD_BSY()
{
    unsigned char status = HDDstatus();
    if ((status & 0x80) == 0x80)
        return 1;
    else
        return 0;
}

int HDD_DRDY()
{
    unsigned char status = HDDstatus();
    if ((status & 0x40) == 0x40)
        return 1;
    else
        return 0;
}

int HDD_DRQ()
{
    unsigned char status = HDDstatus();
    if ((status & 0x08) == 0x08)
        return 1;
    else
        return 0;
}

int HDD_ERR()
{
    unsigned char status = HDDstatus();
    if ((status & 0x01) == 0x01)
        return 1;
    else
        return 0;
}

void HDDread(unsigned int hdd_number, unsigned int sector, char* buffer)
{
	
	unsigned char LBA_a = sector & 0xFF; // sector의 [7:0] 비트 추출
	unsigned char LBA_b = ( sector >> 8 ) & 0xFF; // sector의 [15:8] 비트 추출
	unsigned char LBA_c = ( sector >> 16) & 0xFF; // sector의 [23:16] 비트 추출
    unsigned char LBA_d;
    if (hdd_number)
	    LBA_d = ( (sector >> 24) & 0x0F ) | 0xF0;
    else
        LBA_d = ( (sector >> 24) & 0x0F ) | 0xE0;
    // sector의 [27:24] 비트 추출

	// HDD INT 활성화
	__asm__ __volatile__
	(
		"mov al, 0;"
		"mov dx, 0x3F6;"
		"out dx, al;"
	);

	while ( HDD_BSY() == 1); // HDD가 busy 하다면 계속 대기
	
	/////////////////////////////////////////////////
	// 하드디스크 셋팅 시작
	/////////////////////////////////////////////////

	// 드라이브/헤드 레지스터 초기화 + LBA 주소 [27:24] 4비트
	__asm__ __volatile__
	(
		"mov al, %0;"
		"mov dx, 0x1F6;"
		"out dx, al;"::"r"(LBA_d)
	);

	__asm__ __volatile__
	(
		"mov al, 0x01;"
		"mov dx,0x1F2;"
		"out dx, al;"
	); // 섹터 1개 읽는다

	__asm__ __volatile__
	(
		"mov al, %0;"
		"mov dx,0x1F3;"
		"out dx, al;" ::"r"(LBA_a)
	); // LBA 주소 [7:0] 8비트

	__asm__ __volatile__
	(
		"mov al, %0;"
		"mov dx,0x1F4;"
		"out dx, al;" ::"r"(LBA_b)
	); // LBA 주소 [15:8] 8비트

	__asm__ __volatile__
	(
		"mov al, %0;"
		"mov dx,0x1F5;"
		"out dx, al;" ::"r"(LBA_c)
	); // LBA 주소 [23:16] 8비트

	/////////////////////////////////////////////////
	// 하드디스크 셋팅 끝
	/////////////////////////////////////////////////


	// 읽기(0x20) 내리기 전 하드디스크 드라이버가 명령을 받을 수 있는지 체크
	while ((HDD_BSY() ==1 )|| (HDD_DRDY()==0));


	// 읽기(0x20) 명령 내리기
	__asm__ __volatile__
	(
		"mov al, 0x20;"
		"mov dx, 0x1F7;"
		"out dx, al;"
	);

	// 명령 내렸는데 오류가 발생했다면 읽기를 중단한다.
	if (HDD_ERR() == 1)
	{
		kprintf("Error!!", VIDEOMAXLINE - 1, 0);
		return;
	}

	while (HDD_DRQ() == 0); // 데이터를 다 읽을 때까지 대기


	// 읽기가 성공했으므로 Buffer에다가 512바이트만큼 데이터를 옮긴다.
	__asm__ __volatile__("mov dx,0x1F0;");
	__asm__ __volatile__("mov edi, %0;" : : "r"(buffer));
	__asm__ __volatile__("mov ecx, 256;");
	__asm__ __volatile__("rep insw");
	// __asm__ __volatile__("mov ecx, %0;" : : "r"(sizeof(buffer)));

}

void HDDwrite(unsigned int hdd_number, unsigned int sector, char* buffer)
{

	unsigned char LBA_a = sector & 0xFF; // sector의 [7:0] 비트 추출
	unsigned char LBA_b = (sector >> 8) & 0xFF; // sector의 [15:8] 비트 추출
	unsigned char LBA_c = (sector >> 16) & 0xFF; // sector의 [23:16] 비트 추출
    unsigned char LBA_d;
    if (hdd_number)
	    LBA_d = ( (sector >> 24) & 0x0F ) | 0xF0; // slave hdd write
    else
        LBA_d = ( (sector >> 24) & 0x0F ) | 0xE0; // master disk write

	// HDD INT 활성화
	__asm__ __volatile__
	(
		"mov al, 0;"
		"mov dx, 0x3F6;"
		"out dx, al;"
	);

	while (HDD_BSY() == 1); // HDD가 busy 하다면 계속 대기

	/////////////////////////////////////////////////
	// 하드디스크 셋팅 시작
	/////////////////////////////////////////////////

	// 드라이브/헤드 레지스터 초기화 + LBA 주소 [27:24] 4비트
	__asm__ __volatile__
	(
		"mov al, %0;"
		"mov dx, 0x1F6;"
		"out dx, al;"::"r"(LBA_d)
	);

	__asm__ __volatile__
	(
		"mov al, 0x01;"
		"mov dx,0x1F2;"
		"out dx, al;"
	); // 섹터 1개 쓴다

	__asm__ __volatile__
	(
		"mov al, %0;"
		"mov dx,0x1F3;"
		"out dx, al;" ::"r"(LBA_a)
	); // LBA 주소 [7:0] 8비트

	__asm__ __volatile__
	(
		"mov al, %0;"
		"mov dx,0x1F4;"
		"out dx, al;" ::"r"(LBA_b)
	); // LBA 주소 [15:8] 8비트

	__asm__ __volatile__
	(
		"mov al, %0;"
		"mov dx,0x1F5;"
		"out dx, al;" ::"r"(LBA_c)
	); // LBA 주소 [23:16] 8비트

	   /////////////////////////////////////////////////
	   // 하드디스크 셋팅 끝
	   /////////////////////////////////////////////////


	   // 쓰기(0x30) 내리기 전 하드디스크 드라이버가 명령을 받을 수 있는지 체크
	while ((HDD_BSY() == 1) || (HDD_DRDY() == 0));


	// 쓰기(0x30) 명령 내리기
	__asm__ __volatile__
	(
		"mov al, 0x30;"
		"mov dx,0x1F7;"
		"out dx, al;"
	);

	// 명령 내렸는데 오류가 발생했다면 쓰기를 중단한다.
	if (HDD_ERR() == 1)
	{
		kprintf("Error!!", VIDEOMAXLINE - 1, 0);
		return;
	}

	while (HDD_DRQ() == 0); // 데이터를 쓸 때까지 대기


	//  Buffer의 512바이트만큼 데이터를 메모리(0x1F0)에 쓴다
	__asm__ __volatile__("mov dx,0x1F0;");
	__asm__ __volatile__("mov esi, %0;" : : "r"(buffer));
	__asm__ __volatile__("mov ecx, 256");
	__asm__ __volatile__("rep outsw");
}