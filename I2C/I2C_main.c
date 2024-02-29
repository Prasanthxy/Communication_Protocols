#include"I2Cheader.h" 
#include"UART0header.h" 
#include "Lcd_4bitheader.h"

u8 p[20];

int main() 
{ 
	//u8 ch; 
	init_i2c(); 
	UART0_Config();
	LCD_Config();
	i2c_eeprom_page_write(I2C_EEPROM_SA,0x00,"TheHonoredOne",13); 
	i2c_eeprom_seq_read(I2C_EEPROM_SA,0x00,p,13);
	UART0_String(p);
	LCD_String(p);
 
  
	/*IODIR0=gLED|rLED; 
	i2c_eeprom_write(I2C_EEPROM_SA,0x00,'D'); 
	ch=i2c_eeprom_read(I2C_EEPROM_SA,0x00); 
	UART0_Config();
	UART0_Tx(ch);
 
	if(ch=='D') 
  IOPIN1=gLED; 
 else 
  IOPIN1=rLED;*/ 
 //while(1); 
}
