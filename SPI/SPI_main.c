#include<LPC21XX.H>
#include"LCD_4bitheader.h"
//TYPEDEF 

typedef unsigned int U32;
typedef unsigned char U8;
typedef signed char S8;
typedef float F32;
/********/

//DEFINED FOR CLOCK/BAUDRATE

#define FOSC 12000000
#define CCLK (5*FOSC)// CLK -->60MHz 
#define PCLK (CCLK/4)//PCLK--->15MHz 
#define SPCCR_VAL 60// (5*Fosc) = 60 SPCCR means ---->SPI Clock Counter Register
#define SPI_RATE (unsigned char)(PCLK/SPCCR_VAL)//SPI_RATE--->25000

/****************/

//SPCR register controls the operation of the SPI Setting 
//SPI OPERATING MODES

//7 6 5 4  3 2 1 0 
//8 4 2 1  8 4 2 1
//0 0 0 0 |0 X X X MODE 0 =0X00;
//0 0 0 0 |1 X X X MODE 1 =0X08;
//0 0 0 1 |0 X X X MODE 2 =0X10;
//0 0 0 1 |1 X X X MODE 3 =0X18;

#define Mode_0     0x00  // CPOL 0 CPHA 0
#define Mode_1     0x08  // CPOL 0 CPHA 1
#define Mode_2     0x10  // CPOL 1 CPHA 0
#define Mode_3     0x18  // CPOL 1 CPHA 1

#define MSTR_BIT   5     // SPI0 as Master 
#define LSBF_BIT   6     // default MSB first,if set LSB first
#define SPIE_BIT   7     //SPI Interrupt Enable Bit
/****************/

//CHIP SELECTION MACRO 

#define SETBIT(WORD,BITPOS)         (WORD|=1<<BITPOS)
#define CLRBIT(WORD,BITPOS)         (WORD&=~(1<<BITPOS))
#define CPLBIT(WORD,BITPOS)         (WORD^=(1<<BITPOS))
#define WRITEBIT(WORD,BITPOS,BIT)   (BIT ? SETBIT(WORD,BITPOS): CLRBIT(WORD,BITPOS))
#define READBIT(WORD,BITPOS)        ((WORD>>BITPOS)&1)
/****************/

#define CS 7// PORT PIN 7 USE TO SLAVE SELECTION SO WE ASSIGNED CE 7

/****************/

// SPSR bits
#define SPIF_BIT   7    // Data Transfer Completion Flag

//SPINT bit
#define SPIINTF_BIT 0   //SPI Interrupt Flag Bit

/****************/

void SPI0_CONFIG(void);
float READ_ADC_MCP3204(unsigned char chanel_n0);
unsigned char SPIO_READ(unsigned char);

int main()
{
   float f;
   SPI0_CONFIG();
   LCD_Config();
   LCD_Command(0X01);
   LCD_String("MPC3204");
   while(1)
   {
    //LCD_COMMAND(0X01);
    //LCD_DELAY_MS(10);
    f=READ_ADC_MCP3204(0);
    LCD_Command(0XC0);
    LCD_Float(f);
   }
}
void SPI0_CONFIG(void)
{
  PINSEL0|=0X00001500;// SELECT THE PIN FUNCTIONS P0.4-SCK0 P0.5-MISO P0.6-MOSI P0.7-DEPONDS ON USER WITH CODE
  S0SPCCR=SPI_RATE;//SPI_RATE--->25000
  S0SPCR  = (1<<MSTR_BIT|Mode_3);//MSTR ,LSBF , SPIE ENABLE THE THREE BITS OR 0X0E ALSO USED --->SPI Control Register(SPCR)
  SETBIT(IOPIN0,CS);//P0.7 PORT PIN SET ---> SET means THAT PIN IS 0
  SETBIT(IODIR0,CS);//P0.7 PORT PIN OUTPUT DIRECTION 0 ---> SLAVE SELECT; 1---> DE-SELECT THE SLAVE 
}unsigned char SPI0(unsigned char DATA)
{
  //unsigned char STATUS;
  unsigned char STATUS=S0SPCR;
  S0SPDR=DATA;
  while(READBIT(S0SPSR,SPIF_BIT)==0); // wait for transmission to complete
  return S0SPDR;    // read data from SPI data reg, place into buffer 
}
float READ_ADC_MCP3204(unsigned char chanel_no)
{
  unsigned char lByte,hByte;
 
  unsigned int ADC_VAL=0;
  
  //select/activate chip 
  CLRBIT(IOPIN0,CS);
  //delay_ms(100);
  SPI0(0X06);
  lByte=SPI0(chanel_no<<6);
  hByte=SPI0(0X00);
  
  //de-select/de-activate chp
  SETBIT(IOPIN0,CS);
  //delay_ms(100);
  ADC_VAL=(((hByte&0X0f)<<8)|lByte);
  return ((ADC_VAL*3.3)/4096);
}
