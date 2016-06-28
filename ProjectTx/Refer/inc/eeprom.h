#ifndef _EEPROM_H
#define _EEPROM_H

#include "stm8s.h"

#define         EEPMASS1     0xAE                   //����Կ��1 
#define         EEPMASS2     0x56                   //����Կ��2 
#define         EEPADDR      0x4000                  //eep��ַ��ʼ

void eeprom_read(u16 u16Addr, u8 *pdatas, u8 len);
void eep_word_write(u16 u16Addr, u8 *pdatas, u8 len);
void eep_16bit_write(u16 u16Addr, u16 pdata);
void eep_16bit_read(u16 u16Addr, u16 *pdata);

#endif
