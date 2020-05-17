#include <REG51F380.H>
#include "elock.h"
#include "debounce.h"

#define T0_QAUNTUM 250;
#define DEBOUNCE_TIME (24000/250)

bit DKLOAD,DKLOAD_LAST;
bit DKSET,DKSET_LAST;

virt_key_t key_set;
virt_key_t key_load;

unsigned char debounce_vtime;

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

void Init_Device( void )
{
	PCA0MD = 0;
	FLSCL = 0x90;
	CLKSEL = 0x03;

	XBR0 = 0x01;
	XBR1 = 0x40;

	debounce_vtime = DEBOUNCE_TIME;
	TMOD	=		TMOD 	| 0x02;
	CKCON	=		CKCON |	0x02;
	TH0		=		-T0_QAUNTUM;
	TL0		=		-T0_QAUNTUM;
	IE		=		0x8A;
	TR0	  =   1;
	TMOD	=		TMOD 	| 0x20;
	TH1		=		-250;					//para um periodo de onda de 10ms x 100 para 1 sec
	TL1		=		-250;
	TR1   =   0;
	count = 100;
	
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
	
}

void main( void ) {
	
	Init_Device();
	init_fsm();
	encode_fsm();

	while(1) {
		if(FLOAD){
			TR1 = 0;
			encode_fsm();
			TR0=0;
				FLOAD =0;
			TR0=1;
		}
		
		if(FSET){
			TR1 = 0;
			encode_fsm();
			TR0=0;
				FSET =0;
			TR0=1;
		}
	}
}
