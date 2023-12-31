#ifndef __FUNCTION_H__
#define __FUNCTION_H__

void kprintf(char*, int, int);
char* itoa(int value, char* str, int base);
void kprintf_at(const char* message, int col, int row);
void kprintf_line_clear(int line, int col);
void kprintf_clear_all();

int kstrcmp(char* str1, char*str2);
int kstrlen(char* str1);
#endif