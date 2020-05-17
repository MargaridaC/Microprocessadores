#ifndef __BUTTONS__
#define __BUTTONS__

 typedef union break_window {
	int window;
	char b[2];
}b_wind_t;

typedef struct virt_key {
	b_wind_t key_window;
	char count_ones;
	unsigned char bit_offset;
}virt_key_t;

extern bit my_debounce( virt_key_t* p_key);
extern void InitButtons();
extern void GoButtons();

//extern bit BLOAD();
//extern bit BSET();

#endif
