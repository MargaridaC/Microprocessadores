#pragma large
#include <REG51F380.H>
#include "buttons.h"
#include "pca.h"

#define T0_QAUNTUM 250;
#define DEBOUNCE_TIME (24000/250)
#define RI1 0
#define TI1 1

bit FLOAD, FSET;
bit DKLOAD,DKLOAD_LAST;
bit DKSET,DKSET_LAST;

virt_key_t key_set;
virt_key_t key_load;

unsigned char debounce_vtime;

sbit P2_0 = (P2^0);
sbit P2_1 = (P2^1);

void Timer2 (void) interrupt 5 {
	if(debounce_vtime-- == 0){
		debounce_vtime = DEBOUNCE_TIME;
		DKSET_LAST = DKSET;
		DKSET = my_debounce(&key_set);

		DKLOAD_LAST = DKLOAD;
		DKLOAD = my_debounce(&key_load);

		if(!DKLOAD && DKLOAD_LAST)
			FLOAD = 1;

		if(!DKSET && DKSET_LAST)
			FSET = 1;
	}
}

bit my_debounce( virt_key_t* p_key) {
	char result;
	char pin = (P0 >> p_key->bit_offset) & 1;

	p_key->key_window.window <<=1;
	p_key->key_window.b[1]|=pin;
	p_key->count_ones += pin;
	result = 4 - p_key->count_ones;
	p_key->count_ones -= p_key->key_window.b[0];
	p_key->key_window.b[0]=0;
	pin = (result >>7) & 1;

	return (bit)pin;
}

void InitButtons() {

		key_set.key_window.window = 0x00ff;
		key_set.count_ones=8;
		key_set.bit_offset = 6;

		key_load.key_window.window = 0x00ff;
		key_load.count_ones=8;
		key_load.bit_offset = 7;

		DKLOAD=1;
		DKLOAD_LAST=1;

		DKSET = 1;
		DKSET_LAST = 1;
}

void GoButtons() {
    if(FLOAD == 1) {
      DutyChange(1, 1);
      FLOAD = 0;
    }
    if(FSET == 1) {
      DutyChange(-1, 1);
      FSET = 0;
    }
}
