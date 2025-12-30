#include "beep.h"
#include "Delay.h"
#include "LCD1602.h"
#include "Key.h"
#include "alarm.h"

// 定义静态变量用于非阻塞延时
static unsigned int ringCounter = 0; //计数器
static bit beepState = 0; //蜂鸣器状态
static bit alarmHandled = 0;

void alarmcheck() //检查时钟是否触发
{
    if(DS1302_time[3] == alarmHour && 
           DS1302_time[4] == alarmMinute && 
           DS1302_time[5] == alarmSecond) //闹钟开启且未响铃
    {
        if(alarmEnable && !alarmRinging && !alarmHandled)
        {
            alarmRinging = 1; // 触发闹钟
            ringCounter = 0;  // 重置计数器
            beepState = 0;    // 重置蜂鸣器状态
			alarmHandled = 1;
        }
    }
		 else
    {
        alarmHandled = 0;
    }
}

void alarmRing() 
{
    if(alarmRinging) //闹钟正在响
    {
        ringCounter++;
        
        // 500ms响，500ms停
        if(ringCounter < 500) 
        {
            if(!beepState) { Beep(1); beepState = 1; }
        }
        else if(ringCounter < 1000) 
        {
            if(beepState) { P2_3 = 1; beepState = 0; } // 间歇静音时强制拉高
        }
        else
        {
            ringCounter = 0; 
        }
    }
    else //闹钟没响
    {
        // 强制复位硬件状态，防止引脚卡在低电平一直响
        ringCounter = 0;
        beepState = 0;
        P2_3 = 1; //强制拉高引脚，彻底关闭蜂鸣器
    }
}

void alarmset() //时钟设置
{
    if(KeyNum == 2)
    {
        select++;
        select %= 4; // 0-2:时分秒, 3:使能开关
        KeyNum = 0;
    }
    if(KeyNum == 3)
    {
        if(select == 0) // 设置小时
        {
            alarmHour++;
            if(alarmHour > 23) alarmHour = 0;
        }
        else if(select == 1) // 设置分钟
        {
            alarmMinute++;
            if(alarmMinute > 59) alarmMinute = 0;
        }
        else if(select == 2) // 设置秒钟
        {
            alarmSecond++;
            if(alarmSecond > 59) alarmSecond = 0;
        }
        else if(select == 3) // 切换使能状态
        {
            alarmEnable = !alarmEnable;
        }
        KeyNum = 0;
    }
    if(KeyNum == 4)
    {
        if(select == 0) // 设置小时
        {
            if(alarmHour == 0) alarmHour = 23;
            else alarmHour--;
        }
        else if(select == 1) // 设置分钟
        {
            if(alarmMinute == 0) alarmMinute = 59;
            else alarmMinute--;
        }
        else if(select == 2) // 设置秒钟
        {
            if(alarmSecond == 0) alarmSecond = 59;
            else alarmSecond--;
        }
        KeyNum = 0;
    }
    
    // 显示闹钟时间
    LCD_ShowString(1, 1, "Alarm:");
    if(select == 0 && flash == 1)
    {
        LCD_ShowString(1, 7, "  ");
    }
    else 
    {
        LCD_ShowNum(1, 7, alarmHour, 2);
    }
    
    if(select == 1 && flash == 1)
    {
        LCD_ShowString(1, 10, "  ");
    }
    else 
    {
        LCD_ShowNum(1, 10, alarmMinute, 2);
    }
    
    if(select == 2 && flash == 1)
    {
        LCD_ShowString(1, 13, "  ");
    }
    else 
    {
        LCD_ShowNum(1, 13, alarmSecond, 2);
    }
    
    // 显示使能状态
    LCD_ShowString(2, 1, "Status:");
    if(select == 3 && flash == 1)
    {
        LCD_ShowString(2, 8, "    ");
    }
    else
    {
        if(alarmEnable)
            LCD_ShowString(2, 8, "ON ");
        else
            LCD_ShowString(2, 8, "OFF");
    }

}


