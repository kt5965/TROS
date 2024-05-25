#include "data.h"
#include "shell.h"
#include "function.h"
#include "interrupt.h"

void sh_clear()
{
    kprintf_clear_all();
    curline = -1;
}

void sh_version()
{
	kprintf("TROS version 0.0.1", ++curline, 0);
}

void sh_timer_clear()
{
    timer_ticks = 0;
}

void sh_HDDread()
{
	HDDread(0, diskbuffer);
	kprintf(diskbuffer, ++curline, 0);

	HDDread(1, diskbuffer);
	kprintf(diskbuffer, ++curline, 0);
}

void sh_HDDwrite()
{

	unsigned char test[512] = "Hello world!";

	HDDwrite(0, test);
	HDDwrite(1, test);

}

void sh_arg()
{
	int test[] = { 12, 345, 6789, 12345 };

	kprintf("Test : % and % and % and % and % ", ++curline, 0, test[0], test[1], test[2], test[3], 3);

}
