#include <REG51F380.H>
//#include <stdio.h>
#include "debounce.h"

#define T0_QAUNTUM 250;
#define DEBOUNCE_TIME (24000/250)
#define RI1 0
#define TI1 1

sbit P0_6 = (P0^6);
sbit P0_7 = (P0^7);

bit FLOAD, FSET;
bit DKLOAD,DKLOAD_LAST;
bit DKSET,DKSET_LAST;

virt_key_t key_set;
virt_key_t key_load;

unsigned char debounce_vtime;
unsigned char DutyCycle = 5;
char letrinha = 0;

void Timer0 ( void ) interrupt 1 {

	if(debounce_vtime-- == 0){
		debounce_vtime = DEBOUNCE_TIME;
		DKSET_LAST = DKSET;
		DKSET = my_debounce(&key_set);
		
		DKLOAD_LAST = DKLOAD;
		DKLOAD = my_debounce(&key_load);
		
		if(!DKLOAD && DKLOAD_LAST)
			FLOAD = 1;
		
		if(!DKSET && DKSET_LAST)
			FSET = 1;
	}
}

void getkey ()  interrupt 4{
  char c;

  if (RI0 == 1) {
		RI0 = 0;
		c = SBUF0;
		P2 = c;
	}
	if (TI0 == 1) {
	}
} 


void Init()
{
	  PCA0MD    = 0x00;
	  FLSCL     = 0x90;
    CLKSEL    = 0x03;
    PCA0CN    = 0x40;	
	  PCA0CPM0  = 0x42; 
		
	  PCA0CPL0	= DutyCycle;
    PCA0CPH0  = DutyCycle;

		XBR0 			= 0x01;
		XBR1      = 0x41;
	
		TMOD			= 0x22;
		CKCON			= 0X0C;
		TH1				= 0x30;
	  SCON0			= 0X10;
	
		EIE1			= 0x10;
	  EIP1			= 0x10;
	
		debounce_vtime = DEBOUNCE_TIME;
		TH0				=		-T0_QAUNTUM;
		TL0				=		-T0_QAUNTUM;
		IE			  =		0x92;
		TR0	  		=   1;
		TI0 			= 	1;
		TR1	  		=   1;
		
		EA 				= 1;
		
		key_set.key_window.window = 0x00ff;
		key_set.count_ones=8;
		key_set.bit_offset = 6;

		key_load.key_window.window = 0x00ff;
		key_load.count_ones=8;
		key_load.bit_offset = 7;
		
		DKLOAD=1;
		DKLOAD_LAST=1;
		
		DKSET = 1;
		DKSET_LAST = 1;
		
		P2 = 0;
}




void main( void ) {
		
		Init();
	  while(1) {
			if(FLOAD) {// || letrinha == 'M') {
				DutyCycle -= 10;
				FLOAD = 0;
			}
			
			else if(FSET) {// || letrinha == 'm') {
				DutyCycle += 10;
				FSET = 0;
			}
			if (PCA0CPH0 != DutyCycle) {
				PCA0CPH0 = DutyCycle;
				//printf("%c", DutyCycle);//putchar(DutyCycle);//
			}
		}
}
