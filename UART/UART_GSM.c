#include"UART0header.h" 
//#include"LCD_4bitheader.h" 
#include"Delay.h"
int main() 
{
		//LCD_Config();
		UART0_Config();
	  
	//For checking the GSM module
	UART0_String("AT\r\n");
	delay_seconds(2);
	
	//Configure to sms sending mode
	UART0_String("AT+CMGF=1\r\n");
	delay_seconds(2);
	
	//Sending sms to recieve device
	UART0_String("AT+CMGS=\"+918838482186\"\r\n");
	delay_seconds(2);
	
	//SMS
	UART0_String("Efforts>>Talent");
	delay_seconds(2);
	
	//Terminating the sms
	UART0_Tx(0x1A);  //cntrl+z
	delay_seconds(2);
	
	//Calling the no.
	UART0_String("ATD8838482186;\r\n");
	delay_seconds(2);
	
	//Hang-up the call
	UART0_String("ATH\r\n");
	delay_seconds(2);
	
	//Attending the call
	UART0_String("ATA\r\n");
	delay_seconds(2);
}
