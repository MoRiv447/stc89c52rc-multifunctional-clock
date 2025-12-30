#include <REGX52.H>
#include "LCD1602.h"
#include "DS1302.h"
#include "Delay.h"
#include "Key.h"
#include "Timer0.h"
#include "EEPROM.h"
#include "beep.h"
#include "alarm.h"
#include "dht11.h"
#include "stopwatch.h"
#include "countdown.h"

unsigned char KeyNum,mode,select,flash;
unsigned char alarmHour = 7, alarmMinute = 0, alarmSecond = 0;
unsigned int dht11_ms = 0;
bit dht11_need_update = 0;
bit alarmEnable = 0;
bit alarmRinging = 0;
extern unsigned char Humi_H, Humi_L, Temp_H, Temp_L;

bit dht11_inited = 0;

void LCD_Clear() //清屏函数
{
	unsigned char i;
	for(i=1; i<=2; i++)
	{
		LCD_ShowString(i, 1, "                ");
	}
}

void dht11show() //温湿度显示
{
    unsigned char ret;

    if(!dht11_inited)
    {
        LCD_ShowString(1,1,"Temp:  .        C");
        LCD_ShowString(2,1,"Humi:  .  %");
        dht11_inited = 1;
    }

    if(dht11_need_update)
    {
        dht11_need_update = 0;

        ret = dht11_getdata(&Humi_H,&Humi_L,&Temp_H,&Temp_L);
        if(ret == 0)
        {
            LCD_ShowNum(1,6,Temp_H,2);
            LCD_ShowNum(1,9,Temp_L,2);
            LCD_ShowNum(2,6,Humi_H,2);
            LCD_ShowNum(2,9,Humi_L,2);
        }
        else if(ret == 2)
        {
            LCD_ShowString(1,1,"checksum err ");
        }
        else
        {
            LCD_ShowString(1,1,"dht11 error  ");
        }
    }
}

void timeshow() //时间显示
{
	DS1302_readtime();
	LCD_ShowString(1,3,"-");
	LCD_ShowString(1,6,"-");
	LCD_ShowString(1,10," ");
	LCD_ShowString(1,11," ");
	LCD_ShowString(1,13," ");
	LCD_ShowString(1,14," ");
	LCD_ShowString(2,3,":");
	LCD_ShowString(2,6,":");
	LCD_ShowString(2,9," ");
	LCD_ShowString(2,10," ");
	LCD_ShowNum(1,1,DS1302_time[0],2);
	LCD_ShowNum(1,4,DS1302_time[1],2);
	LCD_ShowNum(1,7,DS1302_time[2],2);
	LCD_ShowNum(2,1,DS1302_time[3],2);
	LCD_ShowNum(2,4,DS1302_time[4],2);
	LCD_ShowNum(2,7,DS1302_time[5],2);
}

void timeset() //时间设置
{
	if(KeyNum==2)
		//按键2调整设置位置
	{
		select++;
		select%=6;
		KeyNum=0;
	}
	if(KeyNum==3)
		//按键3加1
	{
		DS1302_time[select]++;
		if(DS1302_time[0]>99){DS1302_time[0]=0;}
		if(DS1302_time[1]>12){DS1302_time[1]=1;}
		if(DS1302_time[1]==1||DS1302_time[1]==3||DS1302_time[1]==5||DS1302_time[1]==7||DS1302_time[1]==8||DS1302_time[1]==10||DS1302_time[1]==12)
		{
			if(DS1302_time[2]>31){DS1302_time[2]=1;}
		}
		else if(DS1302_time[1]==4||DS1302_time[1]==6||DS1302_time[1]==9||DS1302_time[1]==11)
		{
			if(DS1302_time[2]>30){DS1302_time[2]=1;}
		}
		else if(DS1302_time[1]==2)
		{
			if(DS1302_time[0]%4==0)
			{
				if(DS1302_time[2]>29){DS1302_time[2]=1;}
			}
			else
			{
				if(DS1302_time[2]>28){DS1302_time[2]=1;}
			}
		}
		if(DS1302_time[3]>23){DS1302_time[3]=0;}
		if(DS1302_time[4]>59){DS1302_time[4]=0;}
		if(DS1302_time[5]>59){DS1302_time[5]=0;}
		KeyNum=0;
	}
	if(KeyNum==4)
		//按键4减1
	{
		DS1302_time[select]--;
		if(DS1302_time[0]<0){DS1302_time[0]=99;}
		if(DS1302_time[1]<1){DS1302_time[1]=12;}
		if(DS1302_time[1]==1||DS1302_time[1]==3||DS1302_time[1]==5||DS1302_time[1]==7||DS1302_time[1]==8||DS1302_time[1]==10||DS1302_time[1]==12)
		{
			if(DS1302_time[2]<1){DS1302_time[2]=31;}
			if(DS1302_time[2]>31){DS1302_time[2]=1;}
		}
		else if(DS1302_time[1]==4||DS1302_time[1]==6||DS1302_time[1]==9||DS1302_time[1]==11)
		{
			if(DS1302_time[2]<1){DS1302_time[2]=30;}
			if(DS1302_time[2]>30){DS1302_time[2]=1;}
		}
		else if(DS1302_time[1]==2)
		{
			if(DS1302_time[0]%4==0)
			{
				if(DS1302_time[2]<1){DS1302_time[2]=29;}
				if(DS1302_time[2]>29){DS1302_time[2]=1;}
			}
			else
			{
				if(DS1302_time[2]<1){DS1302_time[2]=28;}
				if(DS1302_time[2]>28){DS1302_time[2]=1;}
			}
		}
		if(DS1302_time[3]<0){DS1302_time[3]=23;}
		if(DS1302_time[4]<0){DS1302_time[4]=59;}
		if(DS1302_time[5]<0){DS1302_time[5]=59;}
		KeyNum=0;
	}
	//设置时闪烁
	if(select==0&&flash==1){LCD_ShowString(1,1,"  ");}
	else {LCD_ShowNum(1,1,DS1302_time[0],2);}
	if(select==1&&flash==1){LCD_ShowString(1,4,"  ");}
	else {LCD_ShowNum(1,4,DS1302_time[1],2);}
	if(select==2&&flash==1){LCD_ShowString(1,7,"  ");}
	else {LCD_ShowNum(1,7,DS1302_time[2],2);}
	if(select==3&&flash==1){LCD_ShowString(2,1,"  ");}
	else {LCD_ShowNum(2,1,DS1302_time[3],2);}
	if(select==4&&flash==1){LCD_ShowString(2,4,"  ");}
	else {LCD_ShowNum(2,4,DS1302_time[4],2);}
	if(select==5&&flash==1){LCD_ShowString(2,7,"  ");}
	else {LCD_ShowNum(2,7,DS1302_time[5],2);}
}

void main()
{
	LCD_Init();
	DS1302_Init();
	Timer0Init();
	LCD_ShowString(1,1,"  -  -  ");
	LCD_ShowString(2,1,"  :  :  ");
	P2_3=1;
	//按下P3_0切换模式
	while(1)
	{
		KeyNum=Key();
		if(alarmRinging && KeyNum != 0)
		{
			alarmRinging = 0; // 清除响铃标志
            // 在alarmcheck里加了alarmHandled
            // 只要还在这一秒内，alarmcheck就不会再次把alarmRinging置为1
			KeyNum = 0;       // 消耗掉按键，不进行后续操作
		}
		if(KeyNum==1)
		{
			if(mode==0)
			{
				mode=1;
				select=0;
				LCD_Clear();
			}
			else if(mode==1)
			{
				DS1302_settime();
				mode=2;
				select=0;
				LCD_Clear();
			}
			else if(mode==2)
			{
				mode=3;
				LCD_Clear();
				dht11_inited = 0;
			}
			else if(mode==3)
			{
				mode=4;
				LCD_Clear();
				stopwatch_running = 0;
				stopwatch_time = 0;
				stopwatch_ms = 0;
			}
			else if(mode==4)
			{
				mode=5;
				LCD_Clear();
				countdown_running = 0;
				countdown_time = 600;
				countdown_ms = 0;
				countdown_finished = 0;
			}
			else if(mode==5)
			{
				mode=0;
				LCD_Clear();
				dht11_inited = 0;
			}
			KeyNum = 0;
		}
		if(mode != 1 && mode != 2)
		{
			DS1302_readtime(); // 在这里读时间，确保任何界面下时间都是新的
			alarmcheck();      // 检查闹钟
		}
		
		// 全局响铃逻辑：负责响，也负责静音
		alarmRing();
		switch (mode)
		{
			case 0:timeshow();break; //时间显示；闹钟准备
			case 1:timeset();break; //时间设置
			case 2:alarmset();break; //闹钟设置
			case 3:dht11show();break; //温湿度显示
			case 4:stopwatchshow();break; //计时
			case 5:countdownshow();break; //倒计时
		}
	}
}

void Timer0_Routine() interrupt 1 //计时器中断
{
    static unsigned int T0Count;
    TL0 = 0x18;
    TH0 = 0xFC;
    T0Count++;
    if(T0Count>=500)
    {
        T0Count=0;
        flash=!  flash;
    }

    dht11_ms++;
    if(dht11_ms >= 1000)
    {
        dht11_ms = 0;
        dht11_need_update = 1;
    }
	
	stopwatch_timer_handler();
	countdown_timer_handler();

}

