#include<LPC21xx.H>
#include<stdio.h>
void UART0_Config(int baudrate);
void UART0_Tx(unsigned char);
unsigned char UART0_Rx(void);
void UART0_String(char*);
void UART0_Integer(int num);
void UART0_Hex(int num);
void UART0_Float(float f);

void UART0_Config(int baudrate)
{
	PINSEL0|=0x00000005; 	//P0.0 as RxD0 and P0.1 as TxD0 UART0 functionality
	U0LCR=0x83;				//8bit Word length and DLAB bit as 1 
	switch(baudrate)
	{
		case 115200:U0DLL=32; break;
		case 921600:U0DLL=4; break;
		default: U0DLL=32;
	}
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
void UART0_String(char*p)
{
	while(*p)
		UART0_Tx(*p++);
}

/*void UART0_String(char *p)
{
	while(*p!=0){
		U0THR=*p;
		while((U0LSR&(1<<5))==0);
		p++;
	}
}*/

void UART0_Integer(int num)
{
	char buf[20];
	sprintf(buf,"%d",num);
	UART0_String(buf);
}
void UART0_Hex(int num)
{
	char buf[20];
	sprintf(buf,"%x",num);
	UART0_String(buf);
}
void UART0_Float(float f)
{
	int temp=0;
	temp=f;
	UART0_Integer(temp);
	temp=(f-temp)*1000;
	UART0_Tx('.');
	UART0_Integer(temp);
}
