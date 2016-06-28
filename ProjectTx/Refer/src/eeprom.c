#include "eeprom.h"

void eeprom_read(u16 u16Addr, u8 *pdatas, u8 len) 
{ 
  u8 *eep=(u8 *)u16Addr; 
  u8 j; 

  for(j=0; j<len; j++) 
  { 
    pdatas[j]=eep[j];         
  } 
} 

void eep_16bit_read(u16 u16Addr, u16 *pdata)
{
  u8 *eep=(u8 *)u16Addr;
  u8 Hdata,Ldata;
  Hdata = *eep;
  Ldata = *(eep+1);
  *pdata = (u16)Hdata<<8;
  *pdata |= Ldata;
}
void eep_16bit_write(u16 u16Addr, u16 pdata)
{
  u8 *eep=(u8*)u16Addr; 
  u8 Hdata,Ldata;
  Hdata = (u8)(pdata>>8);
  Ldata = (u8)pdata;
  
  FLASH->DUKR=EEPMASS1;                //À¯1             √‹‘ø  
  FLASH->DUKR=EEPMASS2;                //À¯2             √‹‘ø 
  while(!(FLASH->IAPSR&0x08));         //µ»¥˝Ω‚√‹æÕ–˜
  FLASH->CR2 |= 0x40;     //               
  FLASH->NCR2 &= ~0x40;   //…Ë÷√◊÷±‡≥Ã 
  
  *eep = Hdata;
  *(eep+1) = Ldata;
  *(eep+2) = 0;
  *(eep+3) = 0;
  while(!(FLASH->IAPSR&0x04)); 
  FLASH->IAPSR &=0xf7;   
}
void eep_word_write(u16 u16Addr, u8 *pdatas, u8 len)
{ 
  u8 *eep=(u8*)u16Addr; 
  u8 j;

  FLASH->DUKR=EEPMASS1;                //À¯1√‹‘ø  
  FLASH->DUKR=EEPMASS2;                //À¯2√‹‘ø 
  while(!(FLASH->IAPSR&0x08));         //µ»¥˝Ω‚√‹æÕ–˜
 
  //DISABLE_MAC_INTERRUPTS();
  FLASH->CR2 |= 0x40;     //               
  FLASH->NCR2 &= ~0x40;   //…Ë÷√◊÷±‡≥Ã 
  for(j=0; j<len; j++) 
  { 
    eep[j] = pdatas[j]; 
    
  }
  while(!(FLASH->IAPSR&0x04)); 
  
  FLASH->IAPSR &=0xf7;    // off the eeprom program
 // ENABLE_MAC_INTERRUPTS();       
}


