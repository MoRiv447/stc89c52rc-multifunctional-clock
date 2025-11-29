#include "EEPROM.h"
#include <intrins.h>
#include <REGX52.H>
#include "Delay.h"

// I2C引脚定义
sbit SDA = P2^0;  // 数据线
sbit SCL = P2^1;  // 时钟线

// 延时函数
void I2C_Delay(void)
{
    _nop_(); _nop_(); _nop_(); _nop_();
    _nop_(); _nop_(); _nop_(); _nop_();
}

// I2C起始信号
void I2C_Start(void)
{
    SDA = 1;
    I2C_Delay();
    SCL = 1;
    I2C_Delay();
    SDA = 0;
    I2C_Delay();
    SCL = 0;
    I2C_Delay();
}

// I2C停止信号
void I2C_Stop(void)
{
    SDA = 0;
    I2C_Delay();
    SCL = 1;
    I2C_Delay();
    SDA = 1;
    I2C_Delay();
}

// I2C发送应答信号
void I2C_Ack(bit ack)
{
    SDA = ack;
    I2C_Delay();
    SCL = 1;
    I2C_Delay();
    SCL = 0;
    I2C_Delay();
    SDA = 1;
}

// I2C等待应答
bit I2C_WaitAck(void)
{
    bit ack;
    
    SDA = 1;
    I2C_Delay();
    SCL = 1;
    I2C_Delay();
    ack = SDA;
    SCL = 0;
    I2C_Delay();
    
    return ack;
}

// I2C发送一个字节
void I2C_SendByte(unsigned char dat)
{
    unsigned char i;
    
    for(i = 0; i < 8; i++)
    {
        SDA = (dat & 0x80) ? 1 : 0;
        dat <<= 1;
        I2C_Delay();
        SCL = 1;
        I2C_Delay();
        SCL = 0;
        I2C_Delay();
    }
}

// I2C接收一个字节
unsigned char I2C_RecvByte(void)
{
    unsigned char i, dat = 0;
    
    SDA = 1;
    for(i = 0; i < 8; i++)
    {
        dat <<= 1;
        SCL = 1;
        I2C_Delay();
        if(SDA) dat |= 0x01;
        SCL = 0;
        I2C_Delay();
    }
    
    return dat;
}

// EEPROM初始化
void EEPROM_Init(void)
{
    SDA = 1;
    SCL = 1;
}

// 向EEPROM写入一个字节
void EEPROM_Write(unsigned char address, unsigned char dat)
{
    I2C_Start();                    // 起始信号
    I2C_SendByte(0xA0);             // 发送设备地址+写命令 (1010 0000)
    I2C_WaitAck();                  // 等待应答
    I2C_SendByte(address);          // 发送内存地址
    I2C_WaitAck();                  // 等待应答
    I2C_SendByte(dat);              // 发送数据
    I2C_WaitAck();                  // 等待应答
    I2C_Stop();                     // 停止信号
    
    // 等待EEPROM内部写操作完成
    Delay(10);
}

// 从EEPROM读取一个字节
unsigned char EEPROM_Read(unsigned char address)
{
    unsigned char dat;
    
    I2C_Start();                    // 起始信号
    I2C_SendByte(0xA0);             // 发送设备地址+写命令
    I2C_WaitAck();                  // 等待应答
    I2C_SendByte(address);          // 发送内存地址
    I2C_WaitAck();                  // 等待应答
    
    I2C_Start();                    // 重新起始信号
    I2C_SendByte(0xA1);             // 发送设备地址+读命令 (1010 0001)
    I2C_WaitAck();                  // 等待应答
    dat = I2C_RecvByte();           // 读取数据
    I2C_Ack(1);                     // 发送非应答信号
    I2C_Stop();                     // 停止信号
    
    return dat;
}