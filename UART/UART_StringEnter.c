#include"UART0header.h" 
#include"LCD_4bitheader.h" 
int main() 
{ 
 unsigned char r[20];
 int i=0; 
 UART0_Config(); 
 LCD_Config(); 
 
 while(1) 
 { 
  r[i]=UART0_Rx(); 
  if(r[i]=='\r')
   break;
  i++;
 }
  LCD_String(r);
  UART0_String(r);

}
