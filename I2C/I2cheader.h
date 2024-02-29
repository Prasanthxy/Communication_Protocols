#include<LPC21xx.H> 
#include"Delay.h" 
 
#define SCL_EN 0x00000010 //P0.2 as SCL  
#define SDA_EN 0x00000040 //P0.3 as SDA 
 
#define CCLK 60000000 //Hz 60MHz 
#define PCLK CCLK/4   //Hz 15MHz 
#define I2C_SPEED 100000//Hz standard Speed 100Kbps 
#define LOADVAL ((PCLK/I2C_SPEED)/2) 
 
#define AA_Bit 2 
#define SI_Bit 3 
#define STO_Bit 4 
#define STA_Bit 5 
#define I2EN_Bit 6 
 
typedef unsigned char u8; 
typedef signed char s8; 
typedef unsigned short u16; 
typedef signed short s16; 
typedef unsigned int u32; 
typedef signed int s32; 
typedef float f32; 
typedef double f64; 
 
#define I2C_EEPROM_SA 0x50 //AT24C08 Slave address [EEPROM Address] 
#define gLED 1<<16 
#define rLED 1<<17 
 
 
void init_i2c(void); 
void i2c_eeprom_write(u8,u8,u8); 
void i2c_Start(void); 
void i2c_write(u8); 
void i2c_stop(void); 
u8 i2c_eeprom_read(u8 slaveaddr,u8 rBuffaddr); 
void i2c_restart(void); 
u8 i2c_nack(void); 
void i2c_eeprom_page_write(u8,u8,u8*,u8); 
void i2c_eeprom_seq_read(u8,u8,u8*,u8); 
u8 i2c_masterack(void); 
 
void init_i2c(void) 
{ 
 PINSEL0|=SCL_EN|SDA_EN; //Selecting SCL & SDA functionality 
 I2SCLL=LOADVAL; 
 I2SCLH=LOADVAL; 
 I2CONSET=1<<I2EN_Bit; //Enabling I2C 
} 
void i2c_eeprom_write(u8 slaveaddr,u8 wBuffaddr,u8 dat) 
{ 
 i2c_Start(); 
 i2c_write(slaveaddr<<1);  //Slave address with write control bit 
 i2c_write(wBuffaddr); 
 i2c_write(dat); 
 i2c_stop(); 
 delay_milliseconds(10); //waiting for stop condition to finish 
} 
u8 i2c_eeprom_read(u8 slaveaddr,u8 rBuffaddr) 
{ 
 u8 dat; 
 i2c_Start(); 
 i2c_write(slaveaddr<<1); //Slave address with write control bit 
 i2c_write(rBuffaddr); 
 i2c_restart(); 
 i2c_write((slaveaddr<<1)|1); //Slave address with read control bit 
 dat=i2c_nack(); 
 i2c_stop(); 
 return dat; //returing data to main() 
} 
void i2c_eeprom_page_write(u8 slaveaddr,u8 wStartBuffaddr,u8*p,u8 nbytes) 
{ 
 u8 i; 
 i2c_Start(); 
 i2c_write(slaveaddr<<1); //Slave address with write control bit 
 i2c_write(wStartBuffaddr); 
 for(i=0;i<nbytes;i++) 
 { 
  i2c_write(p[i]); 
 } 
 i2c_stop(); 
 delay_milliseconds(10); 
} 
void i2c_eeprom_seq_read(u8 slaveaddr,u8 rStartBuffaddr,u8*p,u8 nbytes) 
{ 
 u8 i; 
 i2c_Start(); 
 i2c_write(slaveaddr<<1); 
 i2c_write(rStartBuffaddr); 
 i2c_restart(); 
 i2c_write((slaveaddr<<1)|1); 
 for(i=0;i<nbytes-1;i++) 
 { 
  p[i]=i2c_masterack(); 
 } 
 p[i]=i2c_nack(); 
 p[i+1]='\0'; 
 i2c_stop(); 
 delay_milliseconds(10); 
} 
 
void i2c_Start(void) 
{ 
 I2CONSET=1<<STA_Bit; 
 while(((I2CONSET>>SI_Bit)&1)==0); 
 I2CONCLR=1<<STA_Bit; 
} 
void i2c_write(u8 dat) 
{ 
 I2DAT=dat; 
 I2CONCLR=1<<SI_Bit; 
 while(((I2CONSET>>SI_Bit)&1)==0); 
} 
void i2c_stop(void) 
{ 
 I2CONSET=1<<STO_Bit; 
 I2CONCLR=1<<SI_Bit; 
 //while(((I2CONSET>>SI_Bit)&1)==0); 
} 
void i2c_restart(void) 
{ 
 I2CONSET=1<<STA_Bit; 
 I2CONCLR=1<<SI_Bit; 
 while(((I2CONSET>>SI_Bit)&1)==0); 
 I2CONCLR=1<<STA_Bit; 
} 
u8 i2c_nack(void) 
{ 
 I2CONSET=0X00; 
 I2CONCLR=1<<SI_Bit; 
 while(((I2CONSET>>SI_Bit)&1)==0); 
 return I2DAT; 
} 
u8 i2c_masterack(void) 
{ 
 I2CONSET=0x04; 
 I2CONCLR=1<<SI_Bit; 
 while(((I2CONSET>>SI_Bit)&1)==0); 
 I2CONCLR=0x04; 
 return I2DAT; 
}
