#include <REG51F380.H>
#include <stdio.h>

#define ALEN 16
sbit P0_7 = (P0^7);
sbit P0_6 = (P0^6);
char updateDisp = 1;
int index = 0;

void print_u0(char* p_message);

void Oscillator_Init() {
	PCA0MD = 0;
	XBR1 = 0x40;
	FLSCL = 0x90;
	CLKSEL = 0x03;
}

void UART_Init() {
	XBR0 = 0x01;
	XBR1 = 0x40;
	SCON0 = 0x10;
	TMOD = 0x20;
	CKCON = 0x08;
	TH1 = 0x30;
	TR1 = 1;
	TI0 = 1;
}

void imprime_Array(int *p_buff, int index) {
	index = index & (ALEN-1);
	P2 = 255-p_buff[index];
}

int ler_caracter() {
	  if(RI0 == 1)
				return SBUF0;
		else
			  return -1;
}

code int array_digits[16] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111, 119, 127, 57, 63, 121, 113};
int letra = 0;

void main (void) {
		Oscillator_Init();
		UART_Init();
		
		while(1){
				if(P0_7 == 0){
						while(P0_7 == 0){}
						index ++;
						updateDisp = 1;
				}
				if(P0_6 == 0){
						while(P0_6 == 0){}
						index --;
						updateDisp = 1;
				}
				if(updateDisp) {
						updateDisp = 0;
						imprime_Array(array_digits, index);
			}
			letra = ler_caracter();
		}
}		