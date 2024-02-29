#include"CANheader.h"

int main()
{
	CAN2_MSG m1; //Structure Variables
	CAN_Config();
	UART0_Config(115200);
	while(1)
	{		
		CAN_Rx(&m1);
		if(m1.rtr==0)
		{
			UART0_String("Node-B: DATA-FRAME is received\r\n");
			UART0_Hex(m1.id);
			UART0_Tx(' ');
			UART0_Hex(m1.dlc);
			UART0_Tx(' ');
			UART0_Hex(m1.byteA);
			UART0_Tx(' ');
			UART0_Hex(m1.byteB);
			UART0_String("\r\n");
		 }	
		else
		{
			UART0_String("Node-B: REMOTE-FRAME is received\r\n");
			UART0_Hex(m1.id);
			UART0_Tx(' ');
			UART0_Hex(m1.dlc);
			UART0_Tx(' ');
			UART0_String("\r\n");
		}	
	}
}
