#include<LPC21xx.h>
#include"CANDelay.h"
#include"CAN_UART0header.h"

typedef unsigned int u32;
typedef unsigned char u8;

typedef struct CAN2
{
	u32 id;
	u32 rtr;
	u32 dlc;
	u32 byteA;
	u32 byteB;
}CAN2_MSG;

void CAN_Config(void);
void CAN_Tx(CAN2_MSG m1);
void CAN_Rx(CAN2_MSG *m1);

void CAN_Config(void)
{
	PINSEL1|=0x00014000; //P0.23 as RD2 and P0.24 as TD2 CAN functionality
	VPBDIV=1; //Selecting the Clock speed to 60Mhz
	C2MOD=0x1; //Reset mode --All CAN activities  are disabled
	AFMR=0x2; // Accept all messages
	C2BTR=0x001C001D; //PCLK/16*BAUDRATE  60000000/16*125000 = 30 --> HEX VALUE 1E -->ASPECT -1 SO 1D
	C2MOD=0x0; //CAN Normal mode
}
void CAN_Tx(CAN2_MSG m1)
{
	C2TID1=m1.id; //Assing the message id
	C2TFI1=(m1.dlc<<16); //Assing the DLC to 16th,17th,18th & 19th bits
	if(m1.rtr==0) //Checking for Data Frame
	{
		C2TFI1&=~(1<<30); //Assing the 30th bit as '0' for Data Frame
		C2TDA1=m1.byteA; //Lower 4bytes
		C2TDB1=m1.byteB; //Upper 4bytes
	}
	else
	{
		C2TFI1|=(1<<30); //Remote Frame
	}
	C2CMR=(1<<0)|(1<<5); //0th bit to Start transmission and 5th bit to select the transmit buffer 1
	while((C2GSR&(1<<3))==0);//wait for data transmission complete.
}
void CAN_Rx(CAN2_MSG *m1)
{
	while((C2GSR&0x1)==0); //Checking the Receiver buffer having data
	m1->id=C2RID; //Message id
	m1->dlc=(C2RFS>>16)&0xF; // DLC field
	m1->rtr=(C2RFS>>30)&0x1; // RTR bit
	if(m1->rtr==0){ //if Data Frame
		m1->byteA=C2RDA;
		m1->byteB=C2RDB;
	}
	C2CMR=(1<<2);//free receiver buffer(imp)
}
