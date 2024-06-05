#ifndef __FUNCTION_H__
#define __FUNCTION_H__

void kprintf(char*, int, int, ...);
void printf(char*, ...);
char* itoa(int value, char* str, int base);
void kprintf_at(const char* message, int col, int row);
void kprintf_line_clear(int line, int col);
void kprintf_clear_all();
int kmemcpy(char* dest, char* src, int size);
int kmemset(char* dest, int value, int size);
char* kstrcpy(char* dest, const char* src);
int kstrcmp(char* str1, char*str2);
int kstrlen(char* str1);
void kprintf_arg(const char* format, int target, char** video);
void HDDread(unsigned int hdd_number, unsigned int sector, char* buffer);
void HDDwrite(unsigned int hdd_number, unsigned int sector, char* buffer);

#endif