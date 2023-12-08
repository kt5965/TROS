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

// 간단한 itoa 함수. 숫자를 문자열로 변환
char* itoa(int value, char* str, int base) {
    char* ptr = str, *ptr1 = str;
    char tmp_char;
    int tmp_value;

    // 체크: 인벨리드 베이스
    if (base < 2 || base > 36) { *str = '\0'; return str; }

    do {
        tmp_value = value; // 만약 value가 10 이면
        value /= base; // base 로 나눈 몫을 value로하고
        *ptr++ = "0123456789abcdefghijklmnopqrstuvwxyz"[tmp_value - value * base];
    } while ( value );

    // 음수일 경우 '-' 부호 추가
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

// 화면에 문자를 출력
void kprintf_at(const char* message, int col, int row) {
    // VGA 메모리 시작 주소
    volatile char* video_memory = (volatile char*)0xb8000;
    int offset = (row * 80 + col) * 2; // 2 바이트 당 문자 하나

    // 메시지 출력
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