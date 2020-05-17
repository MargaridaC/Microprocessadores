#include <REG51F380.H>

void Delay(int tempo) {
	
	while(tempo != 0) {
		tempo--;				//1ms
	}
}

char array[8];
char i = 0;

void main() {
	PCA0MD = 0;
	XBR1 = 0x40;
	while(i <= 7) {
		Delay(1000);
		array[i] = i;
		i++;
l nde 	}
}



/*
	indice -> arrary[indice]
*/