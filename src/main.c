#include "function.h"
#include "interrupt.h"

void main() 
{
    char str[20] = "Taerang OS";
    kprintf(str, 11, 5);
	init_intdesc();
}