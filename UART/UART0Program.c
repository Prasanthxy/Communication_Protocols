#include"UART0header.h"
#include"Lcd_4bitheader.h"
int main()
{
	unsigned char rxbytes;
	UART0_Config();
	LCD_Config();
	//UART0_String("THE WEEKND");
	while(1)
	{
		rxbytes=UART0_Rx();
		UART0_Tx(rxbytes);
		LCD_Data(rxbytes);
	}
}
