#pragma large
#include <REG51F380.H>
#include "init.h"
#include "buttons.h"
#include "uart.h"


void main() {
  Init();
  P2 = 255;
  while(1) {
    //encode();
  }
}
