#include <REG51F380.H>
#include <stdio.h>
#define RI1 0
#define TI1 1

sbit P0_7 = (P0^7);
sbit P0_6 = (P0^6);
sbit P2_7 = (P2^7);
sbit P2_6 = (P2^6);

char PB1C = 0;
bit PB1b = 1;

char PB2C = 0;
bit PB2b = 1;


void UART_Init(){
	PCA0MD = 0;
	FLSCL = 0x90;
	CLKSEL = 0x03;
	
	XBR0 = 0x01;
	XBR1 = 0x40;
	SCON0 = 0x10;
	TMOD = 0x20;
	CKCON = 0x08;
	TH1 = 0x30;
	TR1 = 1;
	TI0 = 1;
	
	TMOD |= 0x02;
	CKCON |= 0x02;
	TH0 = 0xF0;
	TL0 = 0xF0;
	TR0 = 1;
	
	IE = 0x92;
}

/*
char putchar (char c)  {
  if (c == '\n')  {
    while (!(SCON1 & (1<<TI1)));
		SCON1 &= ~(1<< TI1);
    SBUF1 = 0x0d;                         // output CR 
  }
  while (!(SCON1 & (1<<TI1)));
  SCON1 &= ~(1<< TI1);
  return (SBUF1 = c);
}
*/
char _getkey ()  {
  char c;

  if (SCON1 & (1<<RI1))
  c = SBUF1;
	
  SCON1 &= ~(1<<RI1);
  return (c);
} 

void Timer0_Interrupt(void) interrupt 1 {
		if(P0_7 == 1 && PB1C < 127)
			PB1C++;
		else if(P0_7 == 0 && PB1C > 0)
			PB1C--;
		if(PB1C == 127)
			PB1b = 1;
		else if(PB1C == 0)
			PB1b = 0;
		
		if(P0_6 == 1 && PB2C < 127)
			PB2C++;
		else if(P0_6 == 0 && PB2C > 0)
			PB2C--;
		if(PB2C == 127)
			PB2b = 1;
		else if(PB2C == 0)
			PB2b = 0;
}


unsigned char code tabela[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1, 0x86,0x8e};

void main (void) {
	char i=0, c;
		PB1C = 0;
		PB2C = 0;
		//Oscillator_Init();
		UART_Init();
		while(1)
		{		
				P2=tabela[i];		
		
				 if (PB1b==1) 
				 {
					 while(PB1b == 1);
					 i++; 
					 if(i < 10)
						printf("%c\n", i+0x30);
					 else
						 printf("%c\n", i - 10 + 'A');
					 if(i==15) 
					 { 
							i=0;
					 }  
				 }
				 if(PB2b==1)
				 {
					 while(PB2b == 1);
						 if(i==0)
					 {
						 i=15;

					 }	 
					 else
					 i--;
					 if(i < 10)
						 printf("%c\n", i+0x30);
						else
							printf("%c\n", i - 10 + 'A');
				 }

			 //c =_getkey();
			
			if((c=='i') || (c=='I'))
					 i++; 
					 if(i < 10)
						printf("%c\n", i+0x30);
					 else
						 printf("%c\n", i - 10 + 'A');
					 if(i==15) 
					 { 
							i=0;
					 }  
				 
			if((c=='d') || (c=='D'))
						 if(i==0)
					 {
						 i=15;

					 }	 
					 else
					 i--;
					 if(i < 10)
						 printf("%c\n", i+0x30);
						else
							printf("%c\n", i - 10 + 'A');
					
			} 				
}		