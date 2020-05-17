#include "elock.h"
#include <REG51F380.H>

#define TENTATIVAS_COFRE 3
#define SIZE_CHAVES 4
#define QUANTOM1 2000

sbit P2_7 = (P2^7);

unsigned char code Numeros[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8};//,0x80,0x90,0x88,0x83,0xc6,0xa1, 0x86,0x8e};

state_t Astate;
state_t Nstate;

pdata state_t Vstate;
char pdata Trys;
char pdata SEC_KEY[SIZE_CHAVES];
int pdata check;

char USR_KEY[SIZE_CHAVES];

bit PrimeiraVez;// = 1;
char index;// = 0;
char Digits;// = 0;
bit FLOAD;
bit FSET;
char tt;
int count1;
int count;

int switch_erros=0;

void Update_Checksum(bit arg) {				//arg = 0 so alterou ou o Trys ou o Vstate
																			//arg = 1 so alterou a SEC_KEY
	if(arg == 0) {
		check = check & 4095;							//limpar o espaço do checksum onde vou escrever
		check += ((Trys+Vstate)<<12);	
	}
	else {
		check = check & 61440;						//limpar o espaço do checksum onde vou escrever
		check += (SEC_KEY[0]<<9);
		check += (SEC_KEY[1]<<6);
		check += (SEC_KEY[2]<<3);
		check += SEC_KEY[3];
	}
	P3 = (check>>8) & 0xff;							//Por a parte alta do checksum num leds que tenho aqui para facil debug
	P1 = check;													//Por a parte baixa tambem em leds
}

bit Check_Checksum() {
	int checkTest = 0;
	checkTest += ((Trys+Vstate)<<12);	
	checkTest += (SEC_KEY[0]<<9);
	checkTest += (SEC_KEY[1]<<6);
	checkTest += (SEC_KEY[2]<<3);
	checkTest += SEC_KEY[3];
	if(check != checkTest)
		return 1;
	return 0;
	
}

void Timer1 ( void ) interrupt 3 {
	count--;
	if (count == 0) {
		P2_7 = !P2_7;
		count = QUANTOM1;
		count1 --;
		if((Astate == OPEN || Astate == ENCRYPT) && count1 == 0) {
			TR1 = 0;
			P2_7 = 1;
			Astate = LOCKED;
			PrimeiraVez = 1;
			encode_fsm();
		}
		else if(Astate == FAIL && count1 == 0) {
			TR1 = 0;
			P2_7 = 1;
		}
	}
}


void Update_Disp(char t) {
	P2 = Numeros[t];
}

static char F_Recover ( void ) {
	Nstate = LOCKED;
	Trys = 0;
	SEC_KEY[0] = 1;
	SEC_KEY[1] = 2;
	SEC_KEY[2] = 3;
	SEC_KEY[3] = 4;
	Update_Checksum(1);
	return 1;
}


static char F_Locked( void ) {
	char ret = 0;
	if (PrimeiraVez == 1) {
		Digits = 0;
		PrimeiraVez = 0;
		index = 0;
	} else if (FSET == 1) {
		index ++;
		index = index&(sizeof(Numeros)-1);
	} else {
		USR_KEY[Digits] = index;
		Digits ++;
		index = 0;
	}
	Update_Disp(index);
	//P1 = Numeros[SEC_KEY[Digits]];
	Nstate = LOCKED;
	
	if(Digits == 4) {
		Nstate = DECRYPT;
		ret = 1;
	}
	return ret;
}


static char F_Decrypt( void ) {
	char j;
	char t = 0;
	for(j = 0 ;j < SIZE_CHAVES; j++)
		if(USR_KEY[j] != SEC_KEY[j])
			t ++;

	if (t == 0)
		Nstate = OPEN;
	else
		Nstate = FAIL;
	
	return 1;
}


static char F_Fail() {
	Trys ++;
	tt = 0;
	if(Trys <= TENTATIVAS_COFRE-1) {//Locked + Delay
		for (tt = 0; tt<Trys ; tt++) {
			P2 = 0x8e;//Numeros[15];
			TR1 = 1;
			count = QUANTOM1;
			count1 = 10;
			Nstate = LOCKED;
			while (TR1 == 1);
		}
	}	else {
		Nstate = BLOCKED;
	}
	PrimeiraVez = 1;
	return 1;
}


static char F_Blocked ( void ) {
	P2 = 0;
	while(Vstate != RECOVERY) {
		if (P0_6 == 0 && P0_7 == 0)
			Vstate = RECOVERY;
	}
	return 1;
}


static char F_Open( void ){
	Astate = ENCRYPT;
	TR1 = 1;
	Trys = 0;
	P2 = Numeros[0];
	count = QUANTOM1;
	count1 = 10;
	return 0;
}


static char F_Encrypt( void ) {
	char ret = 0, ch = 0;
	if (PrimeiraVez == 1) {
		Digits = 0;
		PrimeiraVez = 0;
		index = 0;
	} else if (FSET == 1) {
		index ++;
		index = index&(sizeof(Numeros)-1);
	}
	else {
		Digits ++;
		USR_KEY[Digits-1] = index;
		index = 0;
	}
	P3 = Numeros[USR_KEY[Digits]];
	Update_Disp(index);
	Nstate = ENCRYPT;
	if (Digits == 4)
	{
		char i;
		for (i = 0; i < SIZE_CHAVES; i++)
			SEC_KEY[i] = USR_KEY[i];
		Update_Checksum(1);
		Nstate = LOCKED;
		ret = 1;
	}
	return ret;
}


void init_fsm(void){
	if(Check_Checksum())
		Vstate = BLOCKED;	
	
		//Vstate = 0;
		Astate = Vstate;
		Nstate = Vstate;
		
		Trys        = 0;
		PrimeiraVez = 1;		//KLO
		index       = 0;
		Digits      = 0;
		
		FSET        = 0;
		FLOAD       = 0;
}


void encode_fsm(void){

	char ret = 1;
	Vstate = Astate;
	Update_Checksum(0);
	while(ret) {
		//P1 = Numeros[Astate];
		switch (Astate) {
				case RECOVERY:	ret = F_Recover();
												break;
				case LOCKED: 		ret = F_Locked();
												break;
				case DECRYPT: 	ret = F_Decrypt();
												break;
				case FAIL:			ret = F_Fail();
												break;
				case BLOCKED:		ret = F_Blocked();
												break;
				case OPEN: 			ret = F_Open();
												break;
				case ENCRYPT: 	ret = F_Encrypt();
												break;
				
				default:
					switch_erros++;
					break;
			}
		
			if(ret){
				Astate = Nstate;
				Vstate = Astate;
				Update_Checksum(0);
				PrimeiraVez = 1;
			}
		}
	}