#include <REG51F380.H>
#include "state.h"


void Update_Disp(char t)
{
	P2 = Numeros[t];
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