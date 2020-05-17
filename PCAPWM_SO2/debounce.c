#pragma large
#include "debounce.h"
#include "uart.h"
#include <REG51F380.H>

#define T2_QAUNTUM 250					//tempo do timer 0
#define DEBOUNCE_TIME 10				//(24000/250)

bit FLOAD, FSET;
bit DKLOAD,DKLOAD_LAST;
bit DKSET,DKSET_LAST;

virt_key_t key_set;
virt_key_t key_load;

unsigned char debounce_vtime = DEBOUNCE_TIME;

sbit P3_7 = (P3^7);
sbit P2_7 = (P2^7);
sbit P3_6 = (P3^6);
sbit P2_6 = (P2^6);
sbit P3_0 = (P3^0);
sbit P2_0 = (P2^0);
sbit P0_7 = (P0^7);
sbit P0_6 = (P0^6);

bit first = 1;

char onda [] = {128,152,176,198,217,233,245,252,255,252,245,233,217,198,176,152,128,104,80,58,39,23,11,4,1,4,11,23,39,58,80,104};

void Timer2 ( void ) interrupt  5{
	P2 = P0;
}
