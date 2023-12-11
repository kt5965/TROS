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

// // 화면에 문자를 출력
// void kprintf_at(const char* message, int col, int row) {
//     // VGA 메모리 시작 주소
//     volatile char* video_memory = (volatile char*)0xb8000;
//     int offset = (row * 80 + col) * 2; // 2 바이트 당 문자 하나

//     // 메시지 출력
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

// int HDD_BUSY() // 7번 비트 확인해서 busy 확인
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

// int HDD_READY() // 6번비트 준비 확인 
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

// int HDD_DRQ() // 데이터가 가 전송 준비 되었는지 확인 3번비트
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

// int HDD_ERR() // ERR 확인 0번 비트
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
//     // LBA 주소로 변경
//     unsigned char LBA_a = sector & 0xFF;
//     unsigned char LBA_b = (sector >> 8) & 0xFF;
//     unsigned char LBA_c = (sector >> 16) & 0xFF;
//     // 0xE0 -> LBA 모드를 키기 위한 고정 비트 설정
//     unsigned char LBA_d = ((sector >> 24) & 0xFF) | 0xE0;;

//     __asm__ __volatile__
//     (
//         // 0x3F6 포트로 0 값을 보내 하드디스크 컨트롤러 리셋
//         // dx 레지스터는 IO 포트 주소를 저장하는데 사용
//         // 0x1F0: 데이터 레지스터, 데이터 읽기/쓰기에 사용.
//         // 0x1F1: 에러 레지스터, 에러 코드 읽기에 사용.
//         // 0x1F2: 섹터 카운트 레지스터, 읽거나 쓸 섹터의 수 설정.
//         // 0x1F3-0x1F5: LBA 로우/미들/하이 레지스터, LBA 주소 설정.
//         // 0x1F6: 드라이브/헤드 레지스터, LBA 모드 및 드라이브 선택.
//         // 0x1F7: 커맨드 레지스터, 
//         "mov al, 0;"
// 		"mov dx, 0x3F6;"
// 		"out dx, al;"
//         "nop;"
//     );

// 	__asm__ __volatile__
// 	(
//         // 0x1F2번 포트에 읽을 섹터 수를 설정
// 		"mov al, 0x01;"
// 		"mov dx, 0x1F2;"
// 		"out dx, al;"
//         "nop;"
// 	);
//     __asm__ __volatile__
// 	(
//         // 01F6부터 01F5까지 포트에 LBA 주소의 각 부분을 보내줌
//         // al 레지스터 데이터를 전송하거나 명령을 보내는데 사용
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
//         // 0x1F7번 포트에 읽기명령 0x20을 보냄
// 		"mov al, 0x20;"
// 		"mov dx,0x1F7;"
// 		"out dx, al;"
// 	);

//     __asm__ __volatile__
//     (
//         // 0x1F0번 포트에서 데이터를 읽어와 버퍼에 저장
//         "mov dx,0x1F0;"
//         // edi : 데이터를 저장할 메모리 주소
//         "mov edi, %0;"
//         // 반복명령에서 사용할 카운터를 저장
//         "mov ecx, 256;"
//         "rep insw;" // 16비트 단위로 데이터를 읽어와 edi가 가리키는 메모리 위치에 저장
//         :: "r"(buffer)
//     );
// }

