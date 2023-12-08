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