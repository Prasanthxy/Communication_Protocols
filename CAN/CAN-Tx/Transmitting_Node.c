#include"CANheader.h"

int main()
{
	CAN2_MSG m1,m2; //Structure Variables
	CAN_Config();
	
	//Sending data frame
	m1.id=0x1AF; //11-bit message ID
	m1.rtr=0; // RTR bit=0 for selecting Data frame
	m1.dlc=2; //Data Length Code 2 for 2bytes
	m1.byteA=0xDDCCBBAA;
	m1.byteB=0;
	
	m2.id=0x5;
	m2.dlc=4;
	m2.rtr=1;
	
	while(1)
	{
		CAN_Tx(m1); //Data Frame
		delay_milliseconds(2000);
		CAN_Tx(m2);
		delay_milliseconds(2000);
	}
	
}
