#include "debounce.h"
#include <REG51F380.H>

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