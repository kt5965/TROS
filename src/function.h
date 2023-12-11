#ifndef __FUNCTION_H__
#define __FUNCTION_H__

void kprintf(char*, int, int);
char* itoa(int value, char* str, int base);
void kprintf_at(const char* message, int col, int row);
void kprintf_line_clear(int line, int col);
void kprintf_clear_all();

unsigned char HDDstatus();
int HDD_BUSY();
int HDD_DRQ();
int HDD_ERR();
int HDD_READY();
void HDDread(unsigned int sector, char* buffer);
void HDDwrite(unsigned int sector, char* buffer);
int kstrcmp(char* str1, char*str2);
int kstrlen(char* str1);
#endif