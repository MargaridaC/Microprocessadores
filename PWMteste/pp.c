#include <REG51F380.H>

sbit P2_7 = (P2^7);
sbit P0_7 = (P0^7);
int x = 0;
int y = 0;
int dif = 1;

void INIT() {
	PCA0MD = 0;
	XBR1 = 0x40;
	XBR0 = 0;
	
  CKCON = 0x02;
	CLKSEL = 0x03;
	
	TCON = 0x10;
  TMOD = 0x02;
	TH0 = 251;
	TL0 = 251;
  IE = 0x82;
}

void time0() interrupt 1 {
	x = x+1;
	if(x > 100)
		P2_7 = 1;
	else
		P2_7 = 0;
	if (x == 200) {
		x = 0;
	}
}

void main() {
	INIT();
	while(1){
		if(P0_7 == 1)
			P2_7 = 0;
	}
}