#include<LPC21xx.H>
void UART0_Config(void);
void UART0_Tx(unsigned char);
unsigned char UART0_Rx(void);
void UART0_String(unsigned char*);

void UART0_Config(void)
{
	PINSEL0|=0x00000005; 	//P0.0 as RxD0 and P0.1 as TxD0 UART0 functionality
	U0LCR=0x83;				//8bit Word length and DLAB bit as 1 
	U0DLL=97;				//Setting baudrate 96000
	U0LCR=0X03;				//DLAB bit as 0
}
unsigned char UART0_Rx(void)
{
	while((U0LSR&(1<<0))==0);	//Checking the DR bit
	return U0RBR;
}
void UART0_Tx(unsigned char ch)
{
	while((U0LSR&(1<<5))==0);	//Checking the THRE bit
	U0THR=ch;
}
void UART0_String(unsigned char*p)
{
	while(*p)
		UART0_Tx(*p++);
}
