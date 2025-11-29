#ifndef __ALARM_H__
#define __ALARM_H__

#include <REGX52.H>


// 声明外部变量
extern unsigned char alarmHour, alarmMinute, alarmSecond;
extern bit alarmEnable, alarmRinging;
extern unsigned char KeyNum, select, flash;
extern unsigned char DS1302_time[7];

// 函数声明
void alarmcheck(void);
void alarmRing(void);
void alarmset(void);

#endif