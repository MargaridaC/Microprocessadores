#include <REG51F380.H>

unsigned char code Numeros[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1, 0x86,0x8e};

#define RECOVERY 0
#define LOCKED 1
#define DECRYPT 2
#define FAIL 3
#define BLOCKED 4
#define OPEN 5
#define ENCRYPT 6

#define TENTATIVAS_COFRE 3
#define SIZE_CHAVES 4

sbit P0_7 = (P0^7);
sbit P0_6 = (P0^6);
sbit P2_7 = (P2^7);
sbit P2_6 = (P2^6);

char PB1C = 0;
bit PB1b = 0;
char PB2C = 0;
bit PB2b = 0;

char idata Astate _at_ 0x80;
char idata Nstate _at_ 0x81;
char idata Vstate _at_ 0x82;
char idata Trys  _at_ 0x83;
char idata USR_KEY[SIZE_CHAVES] _at_ 0x90;
char idata SEC_KEY[SIZE_CHAVES] _at_ 0x98;

bit ClicaBotoes = 0;		//KLL
bit KLICKSet = 0;				//KSS
bit KLICKLoad = 0;
bit PrimeiraVez = 1;		//KLO
bit VALID = 0;
char index = 0;
char Digits = 0;
char i = 0;
int count1;
int count;
char tt;

void Update_Disp(char t)
{
	P2 = Numeros[t];
}


void Init_Device( void )
{
	PCA0MD = 0;
	FLSCL = 0x90;
	CLKSEL = 0x03;

	XBR0 = 0x01;
	XBR1 = 0x40;

	TMOD	=		TMOD 	| 0x02;
	CKCON	=		CKCON |	0x02;
	TH0		=		-250;
	TL0		=		-250;
	IE		=	0x82;
	TR0		=   0;
}


void Timer0 ( void ) interrupt 1 {

	if(Astate == OPEN) {
		count1 --;
		if (count1 == 0){
			P2_7 = !P2_7;
			count --;
			count1 = 2000;
		}
		if (count	== 0){
			Nstate = ENCRYPT;
			ClicaBotoes = 0;
			TR0 = 0;
			P2_7 = 1;
		}
	} else if (Astate == FAIL) {
		count1 --;
		if (count1 == 0){
			P2_7 = !P2_7;
			count --;
			count1 = 2000;
		}
		if (count	== 0){
			ClicaBotoes = 0;
			P2_7 = 1;
			TR0 = 0;
		}
	}
}

void F_Recover ( void ) {
	Nstate = LOCKED;
	ClicaBotoes = 0;
	Trys = 0;
}


void F_Locked( void ) {
	if (PrimeiraVez == 1) {
		Digits = 0;
		PrimeiraVez = 0;
		index = 0;
	} else if (KLICKSet == 1) {
		index ++;
	} else {
		Digits ++;
		USR_KEY[Digits - 1] = index;
		index = 0;
	}
	Update_Disp(index);
	P3 = Numeros[SEC_KEY[Digits]];
	Nstate = LOCKED;
	ClicaBotoes = 1;
	if(Digits == 4) {
		ClicaBotoes = 0;
		Nstate = DECRYPT;
	}
}


void F_Decrypt( void ) {
	char j;
	char t = 0;
	for(j = 0 ;j < SIZE_CHAVES; j++)
		if(USR_KEY[j] != SEC_KEY[j])
			t ++;

	if (t == 0){
		VALID = 1;
		Nstate = OPEN;
	}	else {
		VALID = 0;
		Nstate = FAIL;
	}
}


void F_Fail() {
	Trys ++;
	tt = 0;
	if(Trys <= TENTATIVAS_COFRE-1) {//Locked + Delay
		for (tt = 0; tt<Trys ; tt++) {
			P2 = Numeros[15];
			TR0 = 1;
			count = 10;
			count1 = 2000;
			while (TR0 == 1);
			Nstate = LOCKED;
		}
	}	else {
		Nstate = BLOCKED;
	}
	ClicaBotoes = 0;
}


void F_Blocked ( void ) {
	P2 = Numeros[11];
	P2_7 = 0;
	while(1) {
		if (P0_6 == 0 && P0_7 == 0)
			Vstate = RECOVERY;
	}
}


void F_Open( void ){
	Nstate = LOCKED;
	TR0 = 1;
	ClicaBotoes = 1;
	Trys = 0;
	P2 = 163;
	if (PrimeiraVez == 1){
		PrimeiraVez = 0;
		P2_7 = 0;
		count = 10;
		count1 = 2000;
	}
}


void F_Encrypt( void ) {
	if (PrimeiraVez == 1) {
		Digits = 0;
		PrimeiraVez = 0;
		index = 0;
	} else if (KLICKSet == 1)
		index ++;
	else {
		Digits ++;
		USR_KEY[Digits-1] = index;
		index = 0;
	}
	P3 = Numeros[USR_KEY[Digits]];
	Update_Disp(index);
	Nstate = ENCRYPT;
	ClicaBotoes = 1;
	if (Digits == 4)
	{
		for (i = 0; i < SIZE_CHAVES; i++)
			SEC_KEY[i] = USR_KEY[i];
		Nstate = LOCKED;
	}
}

void le_botoes ( void ) {
	/*
	while(!PB1b && !PB2b) {
		if(P0_7 == 1 && PB1C < 127)
			PB1C++;
		if(PB1C == 127)
			PB1b = 1;
			KLICKSet = 1;
		if(P0_6 == 1 && PB2C < 127)
			PB2C++;
		if(PB2C == 127) {
			PB2b = 1;
			KLICKLoad = 1;
		}
	}


	while(PB1b || PB2b) {
		if(P0_7 == 0 && PB1C > 0)
			PB1C--;
		if(PB1C == 0)
			PB1b = 0;

		if(P0_6 == 0 && PB2C > 0)
			PB2C--;
		if(PB2C == 0)
			PB2b = 0;
	}
		*/
	if (P0_7 == 0) {
		KLICKLoad = 1;
		while(P0_7 == 0)
			;
	}
	if (P0_6 == 0) {
		KLICKSet = 1;
		while(P0_6 == 0)
			;
	}
}



void main( void ) {
	Init_Device();
	Trys = 0;
	Vstate = RECOVERY;
	Astate = Vstate;
	Nstate = Vstate;
	SEC_KEY[0] = 1;
	SEC_KEY[1] = 2;
	SEC_KEY[2] = 3;
	SEC_KEY[3] = 4;

	while(1) {
		KLICKLoad = 0;
		KLICKSet = 0;
		while(!KLICKLoad && !KLICKSet && ClicaBotoes)
			le_botoes();

		if(Astate == OPEN && (KLICKSet == 1 || KLICKLoad == 1))
			TR0 = 0;

		if(Astate != Nstate) {
			Astate = Nstate;
			PrimeiraVez = 1;
		}

		Vstate = Astate;
		P1 = Numeros [Astate];

		switch (Astate) {
			case RECOVERY:	F_Recover();
											break;
			case LOCKED: 		F_Locked();
											break;
			case DECRYPT: 	F_Decrypt();
											break;
			case FAIL:			F_Fail();
											break;
			case BLOCKED:		F_Blocked();
											break;
			case OPEN: 			F_Open();
											break;
			case ENCRYPT: 	F_Encrypt();
											break;
		}
	}
}
