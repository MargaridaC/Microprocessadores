#pragma large
#include "buttons.h"
#include <REG51F380.H>

#define T2_QAUNTUM 250					//tempo do timer 0
#define DEBOUNCE_TIME 10				//(24000/250)

bit FLOAD, FSET;
bit DKLOAD,DKLOAD_LAST;
bit DKSET,DKSET_LAST;

unsigned char debounce_vtime = DEBOUNCE_TIME;

//char onda [] = {128,152,176,198,217,233,245,252,255,252,245,233,217,198,176,152,128,104,80,58,39,23,11,4,1,4,11,23,39,58,80,104};

void Timer2 (void) interrupt 5 {
  P2 = P0;
}
