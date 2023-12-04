#include "function.h"
#include "interrupt.h"

void main() 
{
    char str[5] = "Test";
    kprintf(str, 11, 5);
    init_intdesc();
}