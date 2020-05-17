#define RECOVERY 0
#define LOCKED 1
#define DECRYPT 2
#define FAIL 3
#define BLOCKED 4
#define OPEN 5
#define ENCRYPT 6

#define TENTATIVAS_COFRE 4

extern char idata Astate;
extern char idata Nstate;
extern char idata Vstate;
extern char idata Trys;
extern char i;
extern bit SaltaLBotoes;		//KLL


void F_Fail() {
	Trys ++;
	if(Trys <= TENTATIVAS_COFRE-1) {//Locked + Delay
		for(i = 0; i < Trys; i++)
			;//Delay(5);
		Nstate = LOCKED;
	}
	else
		Nstate = BLOCKED;
	SaltaLBotoes = 1;		
}
