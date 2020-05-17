#ifndef __ELOCK_MODULE__
#define __ELOCK_MODULE__

sbit P0_7 = (0x80^7);
sbit P0_6 = (0x80^6);


extern int count1;
extern int count;

typedef enum {RECOVERY = 0,LOCKED,DECRYPT,FAIL,BLOCKED,OPEN,ENCRYPT} state_t;
//						 RECOVERY 0 LOCKED 1 DECRYPT 2 FAIL 3 BLOCKED 4 OPEN 5 ENCRYPT 6

extern state_t Astate;
extern state_t Nstate;
extern pdata state_t Vstate;


extern bit FLOAD;
extern bit FSET;
extern bit PrimeiraVez;// = 1;		//KLO
extern bit entra;

extern void encode_fsm(void);

extern void init_fsm(void);
#endif //__ELOCK_MODULE__