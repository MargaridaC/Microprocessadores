#include <REG51F380.H>

void Init()
{
	PCA0MD 	= 0;
	FLSCL 	= 0x90;
	CLKSEL 	= 0x03;
	
	XBR0 	= 	0x01;
	XBR1 	=	  0x40;
	SCON0 = 	0x10;
	TMOD	=		0x20;
	CKCON	=		0x08;
	TH1		=		0x30;
	TR1		=		1;
	TI0		=		1;
}
