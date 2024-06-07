#ifndef __DATA_H__
#define __DATA_H__

#define VIDEOMAXLINE 25
#define VIDEOMAXCOL 80

#define TRUE 1
#define FALSE 0

#define DEBUG 24


unsigned char keyboard[VIDEOMAXCOL];
unsigned short index;
unsigned short curline;
unsigned short curcol;
unsigned int current_directory;
unsigned char diskbuffer[512];

#endif
