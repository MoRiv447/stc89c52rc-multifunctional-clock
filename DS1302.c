#include <REGX52.H>
sbit DS1302_ae=P1^2;
sbit DS1302_io=P1^1;
sbit DS1302_silk=P1^0;
char DS1302_time[]={25,11,17,13,46,55,1}; //初始值
#define DS1302_second 0x80
#define DS1302_minute 0x82
#define DS1302_hour   0x84
#define DS1302_date   0x86
#define DS1302_month  0x88
#define DS1302_year   0x8C
#define DS1302_day    0x8A
#define DS1302_wp     0x8E
void DS1302_Init() //初始化函数
{
	DS1302_ae=0;
	DS1302_silk=0;
	
}
void DS1302_writebyte(unsigned char command,unsigned char date) //写入位置和数据
{
	unsigned int i;
	DS1302_ae=1;
	for(i=0;i<8;i++)
	{
		DS1302_silk=0;
		DS1302_io=command&(0x01<<i);
		DS1302_silk=1;
	}
	for(i=0;i<8;i++)
	{
		DS1302_silk=0;
		DS1302_io=date&(0x01<<i);
		DS1302_silk=1;
	}
	DS1302_silk=0;
	DS1302_ae=0;
}
unsigned char DS1302_readbyte(unsigned char command) //读出数据
{
	unsigned char i,date=0x00;
	DS1302_ae=1;
	for(i=0;i<8;i++)
	{
		DS1302_silk=0;
		DS1302_io=command&(0x01<<i);
		DS1302_silk=1;
	}
	DS1302_io = 1;
	for(i=0;i<8;i++)
	{
		DS1302_silk=0;
		if(DS1302_io)
		{
			date|=(0x01<<i);
		}
		DS1302_silk=1;
	}
	DS1302_silk=0;
	DS1302_ae=0;
	return date;
}
void DS1302_settime() //写入时间
{
	DS1302_writebyte(DS1302_wp,0x00);
	DS1302_writebyte(DS1302_year,DS1302_time[0]/10*16+DS1302_time[0]%10);
	DS1302_writebyte(DS1302_month,DS1302_time[1]/10*16+DS1302_time[1]%10);
	DS1302_writebyte(DS1302_date,DS1302_time[2]/10*16+DS1302_time[2]%10);
	DS1302_writebyte(DS1302_hour,DS1302_time[3]/10*16+DS1302_time[3]%10);
	DS1302_writebyte(DS1302_minute,DS1302_time[4]/10*16+DS1302_time[4]%10);
	DS1302_writebyte(DS1302_second,DS1302_time[5]/10*16+DS1302_time[5]%10);
	DS1302_writebyte(DS1302_wp,0x80);
}
void DS1302_readtime() //读出时间
{
	unsigned char t;
	t=DS1302_readbyte(DS1302_year+1);
	DS1302_time[0]=t/16*10+t%16;
	t=DS1302_readbyte(DS1302_month+1);
	DS1302_time[1]=t/16*10+t%16;
	t=DS1302_readbyte(DS1302_date+1);
	DS1302_time[2]=t/16*10+t%16;
	t=DS1302_readbyte(DS1302_hour+1);
	DS1302_time[3]=t/16*10+t%16;
	t=DS1302_readbyte(DS1302_minute+1);
	DS1302_time[4]=t/16*10+t%16;
	t=DS1302_readbyte(DS1302_second+1);
	DS1302_time[5]=t/16*10+t%16;
}