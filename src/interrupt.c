#include "interrupt.h"
#include "function.h"

struct IDT inttable[4];
struct IDTR idtr = { 256*8-1,0 };

unsigned char keyt[2] = { 'A', 0 };
unsigned char key[2] = { 'A', 0 };

void init_intdesc()
{

	int i,j;
	unsigned int ptr;
	unsigned short *isr;

	{  // 0x00 : isr_ignore
		ptr = (unsigned int)idt_ignore;
		inttable[0].selector = (unsigned short)0x08;
		inttable[0].type = (unsigned short)0x8E;
		inttable[0].offsetl = (unsigned short)(ptr & 0xFFFF);
		inttable[0].offseth = (unsigned short)(ptr >> 16);

	}

	{  // 0x01 : isr_timer
		ptr = (unsigned int)idt_timer;
		inttable[1].selector = (unsigned short)0x08;
		inttable[1].type = (unsigned short)0x8E;
		inttable[1].offsetl = (unsigned short)(ptr & 0xFFFF);
		inttable[1].offseth = (unsigned short)(ptr >> 16);
	}

	{  // 0x02 : isr_keyboard
		ptr = (unsigned int)idt_keyboard;
		inttable[2].selector = (unsigned short)0x08;
		inttable[2].type = (unsigned short)0x8E;
		inttable[2].offsetl = (unsigned short)(ptr & 0xFFFF);
		inttable[2].offseth = (unsigned short)(ptr >> 16);
	}

	// 물리주소 0x0 번지에 ISR 배치

	for (i = 0; i < 256; i++)
	{
		isr = (unsigned short*)(0x0 + i * 8);
		*isr = inttable[0].offsetl;
		*(isr + 1) = inttable[0].selector;
		*(isr + 2) = inttable[0].type;
		*(isr + 3) = inttable[0].offseth;

	}

	// 타이머 ISR 등록
	{
		isr = (unsigned short*)(0x0 + 8 * 0x20);
		*isr = inttable[1].offsetl;
		*(isr + 1) = inttable[1].selector;
		*(isr + 2) = inttable[1].type;
		*(isr + 3) = inttable[1].offseth;
	}

	// 키보드 ISR 등록

	{
		isr = (unsigned short*)(0x0 + 8 * 0x21);
		*isr = inttable[2].offsetl;
		*(isr + 1) = inttable[2].selector;
		*(isr + 2) = inttable[2].type;
		*(isr + 3) = inttable[2].offseth;
	
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
        : "eax"
    );
    char str[3] = "ab";
    kprintf(str, 15, 3);
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
    char str[11] = "idt_ignore";
	kprintf(str, 5, 2);
	
	__asm__ __volatile__
	(
		"popfd;"
		"popad;"
		"pop ds;"
		"pop es;"
		"pop fs;"
		"pop gs;"
		"iret;"
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
		"iret;"
	);


}

void idt_keyboard()
{

	__asm__ __volatile__
	(
		"push gs;"
		"push fs;"
		"push es;"
		"push ds;"
		"pushad;"
		"pushfd;"
		"in al, 0x60;"
		"mov al, 0x20;"
		"out 0x20, al;"
	);

	kprintf(key, 8, 40);
	key[0]++;

	__asm__ __volatile__
	(
		"popfd;"
		"popad;"
		"pop ds;"
		"pop es;"
		"pop fs;"
		"pop gs;"
		"iret;"
	);

}