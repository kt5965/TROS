#ifndef __FUNCTION_H__
#define __FUNCTION_H__

void kprintf(char*, int, int);
char* itoa(int value, char* str, int base);
void kprintf_at(const char* message, int col, int row);
#endif