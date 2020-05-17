#pragma large
#include <REG51F380.H>
#include "uart.h"

char DutyCycle100 = 50;
char DutyCycle255 = 128;

void DutyChange(signed char change, bit what) {
	DutyCycle100 = (what == 1) ? (DutyCycle100 + change) : change;
}

void DutyCheck() {
  DutyCycle100 = (DutyCycle100 > 100) ? 100 : DutyCycle100;
  DutyCycle100 = (DutyCycle100 < 0)   ? 0   : DutyCycle100;
	DutyCycle255 = (int)DutyCycle100*2.55;
	if (PCA0CPH0 != DutyCycle255) {
		PCA0CPH0 = DutyCycle255;
		print("\nDutyCycle: ", 0);
    print(&DutyCycle100, 1);
    print("% \n", 0);
	}
}
