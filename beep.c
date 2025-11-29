#include <REGX52.H>

#include "Delay.h"
void Beep(unsigned char ms)
{
    unsigned int i;
    for(i=0; i<ms; i++)
    {
        P2_3 = ~P2_3; //蜂鸣器作用
        Delay(1);
    }
}