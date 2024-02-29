#include<LPC21xx.H>
//#include"Delay.h"
#include"LCD_4bitheader.h"

typedef unsigned char u8;

#define CS 1<<7  //Chip Select pin P0.7
#define MSTR 1<<5 //Master Selection bit
#define LSBF 1<<6 //LSB first bit('1') or MSB first bit ('0')
#define SPIE 1<<7 // SPI Interrupt Enable bit

#define SPIF 1<<7 //Data Transfer Completion Flag

#define CCLK 60000000 //Hz 60MHz
#define PCLK (CCLK/4) //Hz 15MHz
#define SPI_Speed (PCLK/60) //Hz 250000Hz

#define MODE0 0x00 //CPOL=0,CPHA=0
#define MODE1 0x08 //CPOL=0,CPHA=1
#define MODE2 0x10 //CPOL=1,CPHA=0
#define MODE3 0x18 //CPOL=1,CPHA=1

//SPI OPERATING MODES

//7 6 5 4  3 2 1 0 
//8 4 2 1  8 4 2 1
//0 0 0 0 |0 X X X MODE0 =0x00;
//0 0 0 0 |1 X X X MODE1 =0x08;
//0 0 0 1 |0 X X X MODE2 =0x10;
//0 0 0 1 |1 X X X MODE3 =0x18;

void SPI_Config(void);
float SPI_ADC(u8);
u8 SPI_Master_Write(u8);

void SPI_Config(void)
{
	PINSEL0|=0x00001500; //Selecting SPI functionality P0.4->SCL,P0.5->MISO,P0.6->MOSI
	S0SPCCR= (u8)250;
	//S0SPCCR= SPI_Speed;
	S0SPCR=MSTR|MODE3; //Selecting as Master & Operating MODE3
	IODIR0=CS; //P0.7 as output pin
	IOSET0=CS;
}
float SPI_ADC(u8 Ch_no)
{
	u8 hb,lb;
	int ADC_val=0;
	IOCLR0=CS; //Start Conditon
	SPI_Master_Write(0x06); //Start->1 SGL->1 (0 FOR DIF) D2=0  
	hb=SPI_Master_Write(Ch_no<<6); //Bit D1 D0 for Channel Selecting & Slave Send Upper half ADC val 4bits
	lb=SPI_Master_Write(0x00); //Dummy write for the slave to send remaining 8bits ADC val
	IOSET0=CS; //Stop Condition
	ADC_val=((hb&(0x0f))<<8|lb); //12bits ADC value 
	return ((((ADC_val)*3.3)/4096)); //voltage conversion 
	
}
u8 SPI_Master_Write(u8 data)
{
	u8 clr;
	clr=S0SPSR;//Read SP status register SPIF flag to clear
	S0SPDR=data;
	while((S0SPSR&SPIF)==0); //Wating for transfer complete
	return S0SPDR;
}
