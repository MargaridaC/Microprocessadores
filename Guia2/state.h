#ifndef __STATE_H__
#define __STATE_H__


#define RECOVERY 0
#define LOCKED 1
#define DECRYPT 2
#define FAIL 3
#define BLOCKED 4
#define OPEN 5
#define ENCRYPT 6

#define TENTATIVAS_COFRE 3
#define SIZE_CHAVES 4

extern unsigned char code Numeros[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1, 0x86,0x8e};


sbit P0_7 = (P0^7);
sbit P0_6 = (P0^6);
sbit P2_7 = (P2^7);
sbit P2_6 = (P2^6);

extern char PB1C = 0;
extern bit PB1b = 0;
extern char PB2C = 0;
extern bit PB2b = 0;

extern char idata Astate _at_ 0x80;
extern char idata Nstate _at_ 0x81;
extern char idata Vstate _at_ 0x82;
extern char idata Trys  _at_ 0x83;
extern char idata USR_KEY[SIZE_CHAVES] _at_ 0x90;
extern char idata SEC_KEY[SIZE_CHAVES] _at_ 0x98;

extern bit ClicaBotoes = 0;		//KLL
extern bit KLICKSet = 0;				//KSS
extern bit KLICKLoad = 0;			
extern bit PrimeiraVez = 1;		//KLO
extern bit VALID = 0;
extern char index = 0;
extern char Digits = 0;
extern char i = 0;
extern int count1;
extern int count;
extern char tt;

extern void F_Recover();
extern void F_Locked();
extern void F_Decrypt();
extern void F_Fail();
extern void F_Blocked();
extern void F_Open();
extern void F_Encrypt();

#endif