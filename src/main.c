#include "function.h"
#include "interrupt.h"
#include "shell.h"
#include "data.h"

void shell();
void translate_shell();

void main() 
{
	init_intdesc();
    kprintf_clear_all();
    shell();
}

void shell()
{
    char path[] = "TROS>>";
    curline = 0;
    curcol = kstrlen(path);

    while (1)
    {
        __asm__ __volatile__("cli");

        if (index != 0 && keyboard[index-1] == 0x13)
        {
            kprintf_line_clear(curline, curcol + index - 1);
            keyboard[index-1] = 0;
            translate_shell();
            
            for (int i = 0; i < VIDEOMAXCOL; i++)
                keyboard[i] = 0;
            curline++;
            index = 0;
        }

        kprintf(path,curline,0);
		kprintf_line_clear(curline, curcol+index);
		kprintf(keyboard, curline, curcol);

		__asm__ __volatile__("sti");
    }
}

void translate_shell()
{
	if (keyboard[0] == 0)
        return;
	if (kstrcmp(keyboard, "clear"))
    {
        sh_clear();
        return;
    }
	if (kstrcmp(keyboard, "version")) 
    {   
        sh_version();
        return;
    }
	kprintf("There is no such command.",++curline, 0);
}