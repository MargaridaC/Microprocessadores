#include <REG51F380.H>
#include "uart.h"

extern update_disp;

static int ler_caracter() {
		int data RET = -1;
		if (RI0 == 1)	{
			RET = SBUF0;
			RI0 = 0;
		}
		return RET;
}

void print(char character) {
		SBUF0 = character;
		while(TI0 == 1)
			TI0 = 0;
}

char CheckSerial(unsigned char index) {
		int caracter = 0;
		caracter = ler_caracter();
		if(caracter != -1) {
			print(caracter);
		}
		return index;
}
