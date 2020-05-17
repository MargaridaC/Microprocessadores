#ifndef __DEBOUNCE_MODULE__
#define __DEBOUNCE_MODULE__

typedef struct virt_key {
	char count;
  char f;
}virt_key_t;

extern bit my_debounce( virt_key_t* p_key);
extern void DebounceInit();
 
//extern void Timer2();

#endif // __DEBOUNCE_MODULE__
