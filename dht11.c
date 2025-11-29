#include "dht11.h"
#include "Delay.h"

sbit dht11_dq=P1^6;
unsigned char Humi_L,Humi_H,Temp_L,Temp_H;


void dht11_delay30us() //30us延迟
{
	unsigned char data i;
	i=12;
	while(--i);
}
void dht11_delay40us() //40us延迟
{
	unsigned char data i;
	i=17;
	while(--i);
}

unsigned char dht11_Init(void) //初始化
{
	dht11_dq=1;
	dht11_dq=0;
	Delay(20);
	dht11_dq=1;
	dht11_delay30us();
	if(dht11_dq==0)
	{
		while(dht11_dq==0);
		while(dht11_dq==1);
		return 0;
	}
	return 1;
}
unsigned char dht11_receive(void)
{
	unsigned i,byte=0x00;
	for(i=0;i<8;i++)
	{
		byte<<=1;
		while(dht11_dq==0);
		dht11_delay40us();
		if(dht11_dq==1)
		{
			byte|=0x01;
		}
		while(dht11_dq==1);
	}
	return byte;
}
unsigned char dht11_getdata(unsigned char* Humi_H,unsigned char* Humi_L,unsigned char* Temp_H,unsigned char* Temp_L)
{
    unsigned char i,buffer[5];
    
    EA = 0;           //关闭总中断，避免 Timer0 打断时序

    if(dht11_Init()==0)
    {
        for(i=0;i<5;i++)
        {
            buffer[i]=dht11_receive();
            if(i==4)dht11_delay40us();
        }
        //用“低 8 位”做校验
        if(buffer[4] == ((buffer[0] + buffer[1] + buffer[2] + buffer[3]) & 0xFF))
        {
            *Humi_H=buffer[0];
            *Humi_L=buffer[1];
            *Temp_H=buffer[2];
            *Temp_L=buffer[3];

            EA = 1;   //恢复总中断
            return 0; // 成功
        }
        else
        {
            EA = 1;   // 校验失败也要恢复中断
            return 2; // 校验错误
        }
    }
    else
    {
        EA = 1;       // 初始化失败也要恢复中断
        return 1;     // 初始化失败
    }
}
