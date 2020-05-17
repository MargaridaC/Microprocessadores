#pragma large
#include <REG51F380.h>
#include "interpeter.h"
#include "uart.h"
#include "pca.h"

#define BUFFERSIZE 16

extern char DutyCycle100;
extern char UARTBuffer[BUFFERSIZE];
extern bit tramaCompleta;

bit StringCompare(char str1[BUFFERSIZE], char str2[BUFFERSIZE]) {
	char c = 0, i = 0;
	for (i = 0; i <= (int)(str2[1] - 47); i++)
		if (str1[i] != str2[i])
			c++;
	P2 = c;
	if (c == 0)
		return 1;
	else
		return 0;
		//teste
}

bit Interpeter() {
	if (StringCompare(UARTBuffer, "T5D OFF")) {
		PCA0CN    = 0x00;
		print("PWM OFF \n", 0);
	}
	else if (StringCompare(UARTBuffer, "T4D ON")) {
		PCA0CN    = 0x40;
		print("PWM ON \n", 0);
	}
	else if (StringCompare(UARTBuffer, "T3D ?")) {
		print("\nDutyCycle: ", 0);
		print(&DutyCycle100, 1);
		print("% \n", 0);
	}
	else if (StringCompare(UARTBuffer, "T3D +")) {
		DutyCycle100++;
	}
	else if (StringCompare(UARTBuffer, "T3D -")) {
		DutyCycle100--;
	}
	else if (UARTBuffer[4] <= 57 && UARTBuffer[4] >= 48) {
		if (UARTBuffer[1] == 51)
			DutyCycle100 = (short)(UARTBuffer[4] - 48);
		else if (UARTBuffer[1] == 53)
			DutyCycle100 = 100;
		else
			DutyCycle100 = (short)(UARTBuffer[4] - 48) * 10 + (UARTBuffer[5] - 48);
	}
	tramaCompleta = 0;
	Reset();
	return 1;
}
