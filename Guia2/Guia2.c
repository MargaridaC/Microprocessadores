#include <REG51F380.H>
#include "state.h"

void Init_Device( void )
{
	PCA0MD = 0;
	FLSCL = 0x90;
	CLKSEL = 0x03;
	
	XBR0 = 0x01;
	XBR1 = 0x40;
	
	TMOD	=		TMOD 	| 0x02;
	CKCON	=		CKCON |	0x02;
	TH0		=		-250;
	TL0		=		-250;
	IE		=	0x82;
	TR0		=   0;
}


void Timer0 ( void ) interrupt 1 {

	if(Astate == OPEN) {
		count1 --;
		if (count1 == 0){
			P2_7 = !P2_7;
			count --;
			count1 = 2000;
		}
		if (count	== 0){
			Nstate = ENCRYPT;
			ClicaBotoes = 0;
			TR0 = 0;
			P2_7 = 1;
		}
	} else if (Astate == FAIL) {
		count1 --;
		if (count1 == 0){
			P2_7 = !P2_7;
			count --;
			count1 = 2000;
		}
		if (count	== 0){
			ClicaBotoes = 0;
			P2_7 = 1;
			TR0 = 0;
		}
	}
}

void le_botoes ( void ) {
	/*
	while(!PB1b && !PB2b) {
		if(P0_7 == 1 && PB1C < 127)
			PB1C++;
		if(PB1C == 127)
			PB1b = 1;
			KLICKSet = 1;
		if(P0_6 == 1 && PB2C < 127)
			PB2C++;
		if(PB2C == 127) {
			PB2b = 1;
			KLICKLoad = 1;
		}
	}
	
	
	while(PB1b || PB2b) {
		if(P0_7 == 0 && PB1C > 0)
			PB1C--;
		if(PB1C == 0)
			PB1b = 0;
		
		if(P0_6 == 0 && PB2C > 0)
			PB2C--;
		if(PB2C == 0)
			PB2b = 0;
	}
		*/
	if (P0_7 == 0) {
		KLICKLoad = 1;
		while(P0_7 == 0)
			;
	}
	if (P0_6 == 0) {
		KLICKSet = 1;
		while(P0_6 == 0)
			;
	}
}



void main( void ) {
	Init_Device();
	Trys = 0;
	Vstate = RECOVERY;
	Astate = Vstate;
	Nstate = Vstate;
	SEC_KEY[0] = 1;
	SEC_KEY[1] = 2;
	SEC_KEY[2] = 3;
	SEC_KEY[3] = 4;
	
	while(1) {
		KLICKLoad = 0;
		KLICKSet = 0; 
		while(!KLICKLoad && !KLICKSet && ClicaBotoes)
			le_botoes();
		
		if(Astate == OPEN && (KLICKSet == 1 || KLICKLoad == 1))
			TR0 = 0;
		
		if(Astate != Nstate) {
			Astate = Nstate;
			PrimeiraVez = 1;
		}
		
		Vstate = Astate;
		P1 = Numeros [Astate];
		
		switch (Astate) {
			case RECOVERY:	F_Recover();
											break;
			case LOCKED: 		F_Locked();
											break;
			case DECRYPT: 	F_Decrypt();
											break;
			case FAIL:			F_Fail();
											break;
			case BLOCKED:		F_Blocked();
											break;
			case OPEN: 			F_Open();
											break;
			case ENCRYPT: 	F_Encrypt();
											break;
		}
	}
}