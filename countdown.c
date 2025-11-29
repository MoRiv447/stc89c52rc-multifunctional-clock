#include <REGX52.H>
#include "LCD1602.h"
#include "beep.h"
#include "Key.h"

bit countdown_running = 0; //运行状态
unsigned int countdown_time = 600; //总时长（初始10min）
unsigned int countdown_ms = 0; //毫秒计数
bit countdown_finished = 0; //完成检测

extern unsigned char KeyNum;

void countdownshow(void)
{
	unsigned char minute, second;
	
	LCD_ShowString(1,1,"Countdown:    ");
	
	if(KeyNum==2)
	{
		countdown_running = !countdown_running; //开启 or 关闭
		KeyNum = 0;
	}
	
	if(KeyNum==3) //加30s
	{
		if(!countdown_running) //若倒计时停止
		{
			countdown_time += 30;
			if(countdown_time > 5999)
			{
				countdown_time = 5999;
			}
		}
		KeyNum = 0;
	}
	
	if(KeyNum==4) //减30s
	{
		if(!countdown_running) //若倒计时停止
		{
			if(countdown_time >= 30)
			{
				countdown_time -= 30;
			}
			else
			{
				countdown_time = 0;
			}
		}
		KeyNum = 0;
	}
	
	minute = countdown_time / 60;
	second = countdown_time % 60;
	
	LCD_ShowString(2,1,"  :  ");
	LCD_ShowNum(2,1,minute,2);
	LCD_ShowNum(2,4,second,2);
	
	if(countdown_finished) //完成时蜂鸣
	{
		LCD_ShowString(1,1,"Time's up!      ");
		Beep(500);
		countdown_finished = 0;
	}
}

void countdown_timer_handler(void) //时间处理
{
	if(countdown_running)
	{
		countdown_ms++;
		if(countdown_ms >= 1000)
		{
			countdown_ms = 0;
			if(countdown_time > 0)
			{
				countdown_time--;
			}
			else
			{
				countdown_running = 0;
				countdown_finished = 1;
			}
		}
	}
}