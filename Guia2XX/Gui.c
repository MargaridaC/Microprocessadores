#include <REG51F380.H>
#include <stdio.h>

#define S_RECOVER 0
#define S_LOCKED 1
#define S_DECRYPT 2
#define S_OPEN 3
#define S_ENCRYPT 4
#define S_FAIL 5
#define S_BLOCKED 6

sbit P0_7 = (P0^7);
sbit P0_6 = (P0^6);
sbit P2_7 = (P2^7);
sbit P2_6 = (P2^6);
char PB1 = 0;
bit PB1b = 1;
char PB2 = 0;
char ASTATE = 0, NSTATE, VALIDSTATE;
bit PB2b = 1;
bit K_SET = 0, K_LOAD = 0, KSS, KLL, KLO;

void Init_Device(void) {
		PCA0MD = 0;
		FLSCL = 0x90;
		CLKSEL = 0x03;
		XBR0 = 0x01;
		XBR1 = 0x40;
	
		SCON0 = 0x10;
		TMOD = 0x02;
		CKCON = 0x02;
		IE = 0x82;
		TH0 = 250;
		TR0 = 250;
		TI0 = 1;
}



void Timer0_Interrupt(void) interrupt 1 {
		if(P0_6 == 1 && PB1 < 127)
			PB1++;
		else if(P0_6 == 0 && PB1 > 0)
			PB1--;
		if(PB1 == 127)
			PB1b = 0;
		else if(PB1 == 0)
			PB1b = 1;
		
		if(P0_7 == 1 && PB2 < 127)
			PB2++;
		else if(P0_7 == 0 && PB2 > 0)
			PB2--;
		if(PB2 == 127)
			PB2b = 0;
		else if(PB2 == 0)
			PB2b = 1;	
}



/*
void state_state(void) {
	if(ASTATE != NSTATE){
		KLO = 1;
		ASTATE = NSTATE;
	}
}
*/


void main (void) {
		Init_Device();
		KLL = 0;
		ASTATE = VALIDSTATE;
		NSTATE = VALIDSTATE;
		while(1) {		
			//if(KLL == 1) {
				while(K_LOAD == 0 && K_SET == 0) {
					if(PB1b == 1)
						K_LOAD = 1;
					if(PB2b == 1)
						K_SET = 1;
				}
			//}
				/*
			else
				state_state();
			if(K_LOAD == 1)
				state_state();
			if(K_SET  == 1)
				KSS = 1;
			
			VALIDSTATE = ASTATE;
			switch(ASTATE) {
				case S_RECOVER:		//RECOVERY();
													break;
				case S_LOCKED :		//LOCKED();
													break;
				case S_DECRYPT :	//DECRYPT();
													break;
				case S_OPEN :			//OPEN();
													break;
				case S_ENCRYPT :	//ENCRYPT();
													break;
				case S_FAIL :			//FAIL();
													break;
				case S_BLOCKED :	//BLOCKED();
													break;
			}
			KSS = 0;*/
			
		}
}