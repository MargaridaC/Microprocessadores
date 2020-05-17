#pragma large

#include <REG51F380.H>
#include <stdio.h>
#include "debounce.h"
#include "uart.h"
#include "InitDev.h"


sbit P3_7 = (P3^7);
sbit P2_7 = (P2^7);
sbit P3_6 = (P3^6);
sbit P2_6 = (P2^6);
sbit P3_0 = (P3^0);
sbit P2_0 = (P2^0);
sbit P0_7 = (P0^7);
sbit P0_6 = (P0^6);

void main( void ) {
		Init();
	  while(1) {
			//Timer2();

			if(FLOADGet()) {
				DutyChange(-1, 1);
				FLOADSet(0);
			}
			else if(FSETGet()) {
				DutyChange(1, 1);
				FSETSet(0);
			}
			//P2_0 = P0_6;
			//P3_0 = P0_7;
			encode();
		}
}
