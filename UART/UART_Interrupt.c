#include"UART0header.h" 
#include"LCD_4bitheader.h"
#define LED 1<<17
void UART0_interrput_isr(void)__irq
{
 UART0_String("Interrupt arrived");
 LCD_String("Interrupt arrived");
 VICVectAddr=0;
}
int main()
{
  UART0_Config(); 
  LCD_Config();
  IODIR0=LED;  
  VICIntSelect=0;
  VICVectAddr0=(int)UART0_interrput_isr;
  VICVectCntl0=(0x20)|6;
  U0IER=0x02;
  VICIntEnable=1<<6;
  U0THR='a';
  while(1)
  {
   IOCLR0=LED;
  }
}
