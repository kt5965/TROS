#include "interrupt.h"
#include "function.h"
#include "data.h"
struct IDT idt[256];
struct IDTR idtr;


unsigned char keyt[2] = { 'A', 0 };
unsigned int key_ticks = 0;
unsigned int timer_ticks = 0;
unsigned int ignore_ticks = 10;
// keyboard driver
unsigned char keybuf;
unsigned char shift = 0;
unsigned char ctrl = 0;

void updateShiftState(unsigned char scanCode) {
    if (scanCode == 0x2A || scanCode == 0x36) {
        shift = 1;
    } else if (scanCode == 0xAA || scanCode == 0xB6) {
        shift = 0;
    }
}

void updateCtrlState(unsigned char scanCode) {
	if (scanCode == 0x1D || scanCode == 0xE0) {
		ctrl = 1;
	} else if (scanCode == 0x9D || scanCode == 0xE0) {
		ctrl = 0;
	}
}


unsigned char transScan(unsigned char target, unsigned char shift)
{
	unsigned char result;
	switch (target) 
	{
		case 0x1E: result = shift ? 'A' : 'a'; break;  // A
		case 0x30: result = shift ? 'B' : 'b'; break;  // B
		case 0x2E: result = shift ? 'C' : 'c'; break;  // C
		case 0x20: result = shift ? 'D' : 'd'; break;  // D
		case 0x12: result = shift ? 'E' : 'e'; break;  // E
		case 0x21: result = shift ? 'F' : 'f'; break;  // F
		case 0x22: result = shift ? 'G' : 'g'; break;  // G
		case 0x23: result = shift ? 'H' : 'h'; break;  // H
		case 0x17: result = shift ? 'I' : 'i'; break;  // I
		case 0x24: result = shift ? 'J' : 'j'; break;  // J
		case 0x25: result = shift ? 'K' : 'k'; break;  // K
		case 0x26: 
			if (ctrl) {
				kprintf_clear_all();

				curline = -1;
			}
			else if (shift) {
				result = 'L';
			}
			else {
				result = 'l';
			}
			break;
		case 0x32: result = shift ? 'M' : 'm'; break;  // M
		case 0x31: result = shift ? 'N' : 'n'; break;  // N
		case 0x18: result = shift ? 'O' : 'o'; break;  // O
		case 0x19: result = shift ? 'P' : 'p'; break;  // P
		case 0x10: result = shift ? 'Q' : 'q'; break;  // Q
		case 0x13: result = shift ? 'R' : 'r'; break;  // R
		case 0x1F: result = shift ? 'S' : 's'; break;  // S
		case 0x14: result = shift ? 'T' : 't'; break;  // T
		case 0x16: result = shift ? 'U' : 'u'; break;  // U
		case 0x2F: result = shift ? 'V' : 'v'; break;  // V
		case 0x11: result = shift ? 'W' : 'w'; break;  // W
		case 0x2D: result = shift ? 'X' : 'x'; break;  // X
		case 0x15: result = shift ? 'Y' : 'y'; break;  // Y
		case 0x2C: result = shift ? 'Z' : 'z'; break;  // Z
		case 0x02: result = shift ? '!' : '1'; break;  // 1
		case 0x03: result = shift ? '@' : '2'; break;  // 2
		case 0x04: result = shift ? '#' : '3'; break;  // 3
		case 0x05: result = shift ? '$' : '4'; break;  // 4
		case 0x06: result = shift ? '%' : '5'; break;  // 5
		case 0x07: result = shift ? '^' : '6'; break;  // 6
		case 0x08: result = shift ? '&' : '7'; break;  // 7
		case 0x09: result = shift ? '*' : '8'; break;  // 8
		case 0x0A: result = shift ? '(' : '9'; break;  // 9
		case 0x0B: result = shift ? ')' : '0'; break;  // 0
		case 0x39: result = ' '; break; // 스페이스
		case 0x0E: result = 0x08; break; // 백스페이스 아스키코드 = 8
		case 0x1C: result = 0x13; break;
		default: result = 0xFF; break;
	}
	return result;
}

void init_intdesc()
{
	// struct IDTR idtr;
	int i,j;	
	unsigned short *isr;

	idtr.size = 256*8-1;
	idtr.addr = (unsigned int)&idt;
	for (int i = 0; i < 256; i++)
	{
		unsigned int ptr = (unsigned int)idt_ignore;
		idt[i].selector = (unsigned short)0x08;
		idt[i].type = (unsigned short)0x8E00;
		idt[i].offsetl = (unsigned short)(ptr & 0xFFFF);
		idt[i].offseth = (unsigned short)(ptr >> 16);
	}

	{  // 32 : isr_timer
		unsigned int ptr = (unsigned int)idt_timer;
		idt[32].selector = (unsigned short)0x08;
		idt[32].type = (unsigned short)0x8E00;
		idt[32].offsetl = (unsigned short)(ptr & 0xFFFF);
		idt[32].offseth = (unsigned short)(ptr >> 16);
	}

	{  // 33 : isr_keyboard
		unsigned int ptr = (unsigned int)idt_keyboard;
		idt[33].selector = (unsigned short)0x08;
		idt[33].type = (unsigned short)0x8E00;
		idt[33].offsetl = (unsigned short)(ptr & 0xFFFF);
		idt[33].offseth = (unsigned short)(ptr >> 16);
	}

	for (int i = 0; i < VIDEOMAXCOL; i++) {
		keyboard[i] = 0;
	}
	// x86 아키텍처에서 하드웨어와의 통신에 사용되는 언어
	// 0xAE를 AL 레지스터에 이동(일반 목적 레지스터)
	// 0xAE는 키보드 인터페이스 활성화
	// out은 AL 레지스터의 내용을 I/O 포트 64번에 출력
	__asm__ __volatile__
	(
		"mov al, 0xAE;"
		"out 0x64, al;"
	);

	//  인터럽트 작동 시작

    __asm__ __volatile__
    (
        "mov eax, %0;"  // idtr의 주소를 eax에 로드
        "lidt [eax];"   // IDTR 레지스터에 IDT의 위치를 로드
		"mov al, 0x00;"
		"out 0xA1, al;"
        "mov al, 0x00;" // 인터럽트 마스크 설정
        "out 0x21, al;" // PIC에 인터럽트 마스크 값을 전달
        "sti"           // 인터럽트 활성화
        : 
        : "r"(&idtr)
    );
}

void idt_ignore()
{

	__asm__ __volatile__
	(
		"push gs;"
		"push fs;"
		"push es;"
		"push ds;"
		"pushad;"
		"pushfd;"
		"mov al, 0x20;"
		"out 0x20, al;"
	);
	
	__asm__ __volatile__
	(
		"popfd;"
		"popad;"
		"pop ds;"
		"pop es;"
		"pop fs;"
		"pop gs;"
		"mov esp, ebp;"
        "pop ebp;"
		"nop;"
		"iretd;"
	);
}

void idt_timer()
{
	__asm__ __volatile__
	(
		"push gs;"
		"push fs;"
		"push es;"
		"push ds;"
		"pushad;"
		"pushfd;"
		"mov al, 0x20;"
		"out 0x20, al;"
		
	);
	char tick_str[30];
	timer_ticks++;
    itoa(timer_ticks, tick_str, 10);
	kprintf_at(tick_str, VIDEOMAXCOL-10, VIDEOMAXLINE-1);
	__asm__ __volatile__
	(
		"popfd;"
		"popad;"
		"pop ds;"
		"pop es;"
		"pop fs;"
		"pop gs;"
		"mov esp, ebp;"
        "pop ebp;"
		"nop;"
		"iretd;"
	);


}

void idt_keyboard()
{
	__asm__ __volatile__
	(
		"pushad;"
		"push gs;"
		"push fs;"
		"push es;"
		"push ds;"
		"pushfd;"
		"xor al,al;"
		"in al, 0x60;"
	);
	__asm__ __volatile__("mov %0, al;" :"=r"(keybuf) );
	updateShiftState(keybuf);
	updateCtrlState(keybuf);
	keybuf = transScan(keybuf, shift);

	if (keybuf == 0x08 && index != 0)
		keyboard[--index] = 0;
	else if (keybuf != 0xFF && keybuf !=0x08)
		keyboard[index++] = keybuf;

	__asm__ __volatile__
	(
		"mov al, 0x20;"
		"out 0x20, al;"
		"popfd;"
		"pop ds;"
		"pop es;"
		"pop fs;"
		"pop gs;"
		"popad;"
		"mov esp, ebp;"
		"pop ebp;"
		"nop;"
		"iretd;"
	);

}
