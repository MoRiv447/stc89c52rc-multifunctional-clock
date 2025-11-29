#ifndef __DHT11_H__
#define __DHT11_H__

#include <REGX52.H>

extern unsigned char Humi_L, Humi_H, Temp_L, Temp_H;

void dht11_delay30us();
void dht11_delay40us();
unsigned char dht11_Init(void);
unsigned char dht11_receive(void);
unsigned char dht11_getdata(unsigned char* Humi_H,unsigned char* Humi_L,unsigned char* Temp_H,unsigned char* Temp_L);
#endif