#include <REG51F380.H>
#include <stdio.h>

	const char code array_digit[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E};

	int data index = 0;
	#define ALEN (sizeof(array_digit))
		
	bit PB1_state = 0;
	bit PB1_state_N = 0;
	bit PB2_state = 0;
	bit PB2_state_N = 0;

	/*bits de utilizador*/
	sbit P0_7 = (P0^7);
	sbit P0_6 = (P0^6);
	
/* Fun??o macro para fazer a mascara e atribuir o valor a ser colocado no disp*/
#define M_IMP(x, y)\
y = y & (ALEN-1);\
P2 = x[y];
//printf("%d\n", y);

void Oscillator_Init()
//Inicializa o micro a 48MHz
{
	XBR1 	=	  0x40;
	PCA0MD 	= 0;
	FLSCL 	= 0x90;
	CLKSEL 	= 0x03;
}

void Timer0_Init()
	//Inicializa o timer 0:
	//sysclk = 48MHz ; prescale = 48; frequencia do rel?gio = 1MHz
{

	TMOD	=		TMOD 	| 0x02;		/*Timer 0 em modo 8 bit auto-reaload*/
	CKCON	=		CKCON |	0x02;		/*Define a frequ?ncia do timer*/
	TH0		=		-250;						/*Vai contar 250 us*/
	TL0		=		-250;
	IE			=	0x82;		/*Enable ? interrup??o do timer 0*/
	TR0		= 1;
}

void ISR_TIMER0()	interrupt 1 using 2
{
	static int data contador = 0;
	static unsigned int d_PB1 = 0;  //acho que posso trocar o int por um char
	static unsigned int d_PB2 = 0;
	PB1_state_N = PB1_state;
	PB2_state_N = PB2_state;
	

	
	
	if(++contador == 20)  //40
	{
		contador = 0;
				if (P0_7 == 0)
				{
					if (d_PB1++ == 0x0F) // 0x0F
					{
						d_PB1 = ~d_PB1 & (ALEN-1);
						PB1_state = 1;						/*Colocando aqui "1" invertemos a l?gica e agora se o PB1_state estiver a 1 o but?o est? pressionado*/
					}
					else
					{
						PB1_state = PB1_state;
					}
				}
				else
				{
					if (d_PB1-- == 0x00)
					{
						d_PB1 = ~d_PB1 & (ALEN-1);
						PB1_state = 0;						/*Colocando aqui "0" invertemos a l?gica e agora se o PB1_state estiver a 0 o but?o n?o est? pressionado*/
					}
					else
					{
						PB1_state = PB1_state;
					}
				}
				
				/*_______butao PB2_______*/
				if (P0_6 == 0)
				{
					if (d_PB2++ == 0x0F) // 0x0F
					{
						d_PB2 = ~d_PB2 & (ALEN-1);
						PB2_state = 1;						/*Colocando aqui "1" invertemos a l?gica e agora se o PB1_state estiver a 1 o but?o est? pressionado*/
					}
					else
					{
						PB2_state = PB2_state;
					}
				}
				else
				{
					if (d_PB2-- == 0x00)
					{
						d_PB2 = ~d_PB2 & (ALEN-1);
						PB2_state = 0;						/*Colocando aqui "0" invertemos a l?gica e agora se o PB1_state estiver a 0 o but?o n?o est? pressionado*/
					}
					else
					{
						PB2_state = PB2_state;
					}
				}
	}
}


void main (void)
{
		bit update_disp = 0;

//____Fun??es/Condi??es de inicializa??o do micro______	
		Oscillator_Init();
		Timer0_Init();
 		P2 = 0xC0;
	
//___Testa, infinitamente, os bot?es (polling) para saber se incrementa ou decrementa o valor 
		while(1)
		{
			if (PB1_state && !PB1_state_N)						
			{
				while (!(PB1_state_N && !PB1_state)){}
				index ++;
				update_disp = 1;
			}
			if (PB2_state && !PB2_state_N)						
			{
				
				while (!(PB2_state_N && !PB2_state)){}
				index --;
				update_disp = 1;
			}		
			if(update_disp) //caso algum bot?o tenha sido pressionado atualiza o disp.
			{
				update_disp = 0;
				//Imprime_display(array_digit, index);
				M_IMP(array_digit, index);
			}
		}
}