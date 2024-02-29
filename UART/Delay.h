void delay_seconds(unsigned char s)
{
	T0PR=15000000-1;
	T0TCR=0x01;			//Enable timer
	while(T0TC<s);
	T0TCR=0x03;			//Enable and Reset the timer
	T0TCR=0x00;
}
void delay_milliseconds(unsigned char ms)
{
	T0PR=15000-1;
	T0TCR=0x01;
	while(T0TC<ms);
	T0TCR=0x03;
	T0TCR=0x00;
}
void delay_microseconds(unsigned char us)
{
	T0PR=15-1;
	T0TCR=0x01;
	while(T0TC<us);
	T0TCR=0x03;
	T0TCR=0x00;
}
