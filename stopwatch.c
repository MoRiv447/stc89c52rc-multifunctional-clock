#include <REGX52.H>
#include "LCD1602.h"
#include "Key.h"

bit stopwatch_running = 0; //运行状态
unsigned int stopwatch_time = 0; //总时长（秒）
unsigned int stopwatch_ms = 0; //毫秒计数

extern unsigned char KeyNum;

void stopwatchshow(void) //显示部分
{
	unsigned char minute, second;
	
	LCD_ShowString(1,1,"Stopwatch:    ");
	
	if(KeyNum==2)
	{
		stopwatch_running = !  stopwatch_running;
		KeyNum = 0;
	}
	
	minute = stopwatch_time / 60;
	second = stopwatch_time % 60;
	
	LCD_ShowString(2,1,"  :  ");
	LCD_ShowNum(2,1,minute,2);
	LCD_ShowNum(2,4,second,2);
}

void stopwatch_timer_handler(void) //时间处理
{
	if(stopwatch_running)
	{
		stopwatch_ms++;
		if(stopwatch_ms >= 1000)
		{
			stopwatch_ms = 0;
			stopwatch_time++;
			if(stopwatch_time > 5999)
			{
				stopwatch_time = 5999;
				stopwatch_running = 0;
			}
		}
	}
}