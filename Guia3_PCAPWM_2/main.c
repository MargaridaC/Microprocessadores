#pragma large
#include <REG51F380.H>
#include "init.h"
#include "buttons.h"
#include "uart.h"
#include "pca.h"
#include "interpeter.h"

sbit P2_7 = (P2^7);
bit tramaCompleta;

void Encode() {
  char a = 0;
  a = CheckSerial();
  DutyCheck();
  if(tramaCompleta)
    Interpeter();
}

void main() {
  Init();
  while(1) {
    GoButtons();
    Encode();
  }
}
