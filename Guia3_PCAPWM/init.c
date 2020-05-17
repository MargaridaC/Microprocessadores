#pragma large
#include <REG51F380.H>
#include "buttons.h"

#define CPS1 2

void Init()
{
  PCA0MD  = 0x00;
	PCA0MD = (1<<CPS1); //010

  FLSCL     = 0x90;
  CLKSEL    = 0x03;

  PCA0CPM0  = 0x42;

  PCA0CPL0	= 5;
  PCA0CPH0  = 5;

  XBR0 		= 0x01;
	XBR1 		= 0x41;

	TMOD			= 0x22;
	CKCON			= 0X0C;
	TH1				= 0x30;
  SCON0			= 0X10;

	EIE1			= 0x10;
  EIP1			= 0x10;

	TL0 			= -10;
	TH0 			= -10;

  TMR2CN  = 0x04;
  TMR2RLL = -0xFF;
  TMR2L   = -0xFF;
  TMR2RLH = -0xFF;
  TMR2H   = -0xFF;

  IE			  =		0xB0;//10110000
	TR0	  		=   1;
	TI0 			= 	1;
	TR1	  		=   1;
	TR2 			= 	1;

	PCA0CN    = 0x40;
	EA 				= 1;

}
