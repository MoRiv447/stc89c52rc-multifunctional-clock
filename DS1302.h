#ifndef __DS1302_H__
#define __DS1302_H__
extern char DS1302_time[7];
void DS1302_Init();
void DS1302_writebyte(unsigned char command,unsigned char date);
unsigned char DS1302_readbyte(unsigned char command);
void DS1302_settime();
void DS1302_readtime();
#endif