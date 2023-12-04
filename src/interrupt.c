#include "interrupt.h"
#include "function.h"

struct IDT idt[256];
struct IDTR idtr;
unsigned char keyt[2] = { 'A', 0 };
unsigned int key_ticks = 0;
unsigned int timer_ticks = 0;
unsigned int ignore_ticks = 10;
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
	//  인터럽트 작동 시작
    __asm__ __volatile__
    (
        "mov eax, %0;"  // idtr의 주소를 eax에 로드
        "lidt [eax];"   // IDTR 레지스터에 IDT의 위치를 로드
        "mov al, 0xFC;" // 인터럽트 마스크 설정
        "out 0x21, al;" // PIC에 인터럽트 마스크 값을 전달
        "sti"           // 인터럽트 활성화
        : 
        : "r"(&idtr)
    );
	kprintf("test", 2, 5);
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
	char tick_str[30];
	ignore_ticks++;
    itoa(ignore_ticks, tick_str, 3);
	kprintf_at(tick_str, 5, 2);


	__asm__ __volatile__
	(
		"popfd;"
		"popad;"
		"pop ds;"
		"pop es;"
		"pop fs;"
		"pop gs;"
		"leave;"
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
    itoa(timer_ticks, tick_str, 3);
	kprintf_at(tick_str, 5, 5);
	kprintf(keyt, 7, 40);
	keyt[0]++;
	__asm__ __volatile__
	(
		"popfd;"
		"popad;"
		"pop ds;"
		"pop es;"
		"pop fs;"
		"pop gs;"
		"leave;"
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

	);
	char tick_str[30];
	key_ticks++;
    itoa(key_ticks, tick_str, 3);
	kprintf_at(tick_str, 3, 6);
	__asm__ __volatile__
	(
		"in al, 0x60;"
		"mov al, 0x20;"
		"out 0x20, al;"
		"popfd;"
		"pop ds;"
		"pop es;"
		"pop fs;"
		"pop gs;"
		"popad;"
		"leave;"
		"nop;"
		"iretd;"
	);

}