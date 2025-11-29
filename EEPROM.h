#ifndef __EEPROM_H__
#define __EEPROM_H__

void EEPROM_Init(void);
void EEPROM_Write(unsigned char address, unsigned char dat);
unsigned char EEPROM_Read(unsigned char address);

#endif