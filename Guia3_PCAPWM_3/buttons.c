#pragma large
#include "buttons.h"
#include <REG51F380.H>

void Timer2 (void) interrupt 5 {
  P4 = P0;
}
