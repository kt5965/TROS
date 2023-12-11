#include "data.h"
#include "shell.h"
#include "function.h"

void sh_clear()
{
    kprintf_clear_all();
    curline = -1;
}

void sh_version()
{
	kprintf("TROS version 0.0.1", ++curline, 0);
}

void sh_HDDread()
{
    // char test[] = "test";
    // HDDread(0, diskbuffer);
    // kprintf("test", ++curline, 0);
    // HDDread(1, diskbuffer);
    // kprintf(diskbuffer, ++curline, 0);
}

void sh_HDDwrite()
{
    // char test[] = "test";
    // HDDwrite(0, diskbuffer);
    // kprintf("test", ++curline, 0);
    // HDDwrite(1, diskbuffer);
    // kprintf(diskbuffer, ++curline, 0);
}