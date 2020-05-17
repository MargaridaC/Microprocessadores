#include <REG51F380.H>
#include <stdio.h>
#include "UART.h"

#define DEBUG 1
#define EMPTY 95

sbit P0_6 = (P0^6);
sbit P0_7 = (P0^7);
sbit P2_7 = (P2^7);

char ReadQ = 0;
char func = 255;
char i;

unsigned char letrinha = 0;
char DutyCycle100 = 5;
char DutyCycle255 = 5;
int Recieved = 0;
int RecievedAnt = 0;
unsigned char UARTBuffer[8];
bit flagTrama = 0, TramaCompleta = 0;
int Read = 0;
unsigned char UARTBufferindex = 0;
unsigned char UARTFunction[8];

void encode () {
			Debug();
			if (TramaCompleta == 1)
				ReadUARTBuff();
			if (flagTrama == 1) {
				ApplyFunction();
				Reset();
			}
}

void Reset(bit flagTrama, char UARTBuffer[8], char Recieved, char Read, char UARTBufferindex, char UARTFunction[8]) {
	flagTrama = 0;
	Recieved = 0;
	UARTBufferindex = 0;
	Read = 0;
	for (i = 0; i < 8; i++) {
		UARTFunction[i] = EMPTY;
		UARTBuffer[i] = EMPTY;
	}
}

char StringCompare(char str1[8], char str2[8]) {
	char c = 0;
	for (i = 0; i <= (int)(str2[1] - 47); i++)
		if (str1[i] != str2[i])
			c++;
	if (c == 0)
		return 1;
	else
		return 0;
}

void ApplyFunction(char DutyCycle100, char UARTFunction[8]) {
	
	if (StringCompare(UARTFunction, "T5D OFF")) {
		printf("PWM OFF \n");
	} else if (StringCompare(UARTFunction, "T4D ON")) {
		printf("PWM ON \n");
	} else if (StringCompare(UARTFunction, "T3D ?")) {
		printf("DutyCycle: %d\n", (short)DutyCycle100);
	} else if (StringCompare(UARTFunction, "T3D +")) {
		DutyCycle100++;
	} else if (StringCompare(UARTFunction, "T3D -")) {
		DutyCycle100--;
	} else if (UARTFunction[4] <= 57 && UARTFunction[4] >= 48) {
		if(UARTFunction[1] == 51)
			DutyCycle100 = 0;
		else if(UARTFunction[1] == 53)
			DutyCycle100 = 100;
		else
			DutyCycle100 = (UARTFunction[4]-48)*10 + (UARTFunction[5]-48);	
	}	
}

void AddToUARTBuff(char add, char UARTBuffer[8], char UARTBufferindex) {
	UARTBuffer[UARTBufferindex] = add;
	UARTBufferindex++;
	UARTBufferindex &= 8-1;
}

void ReadUARTBuff(bit TramaCompleta, char UARTBuffer[8], bit flagTrama, char Read, char UARTFunction[8]) {
	char ReadIndex = 0;
	char count = 0, i = 0, TramaSize = 0;
	char vec[8];
	unsigned char rcv;
	
	printf("\n");
	while(ReadIndex < Read)  {
		TramaCompleta = 0;
		rcv=UARTBuffer[ReadIndex];
		ReadIndex++;
		if(count==0 && (rcv==84 || rcv ==116)) {
      vec[count]=rcv;
      count++;
    }
    else if(count==1)  {
      vec[count]=rcv;
			TramaSize = rcv - 48;
      count++;
    }
    else if(count>0) {
      vec[count]=rcv;
      count++;
      if(count>=(TramaSize+3)) {
        count=0;
        if(vec[TramaSize+2]==13 && flagTrama==0) {
            for(i=0;i<=TramaSize+2;i++) {
							UARTFunction[i]=vec[i];
            }
            flagTrama=1;
				}	else
					printf("Comando Errado ou Inexistente\n");
      }
    }
  }
}

void DutyCheck() {
	
			if(DutyCycle100 > 100)
				DutyCycle100 = 100;
			if(DutyCycle100 < 0)
				DutyCycle100 = 0;
			DutyCycle255 = (int)DutyCycle100*2.55;
			if (PCA0CPH0 != DutyCycle255) {
				PCA0CPH0 = DutyCycle255;
				printf("%d\n", (short)DutyCycle100);
			}
}


void getkey ()  interrupt 4{
	char i = 0;
  if (RI0 == 1) {
		RI0 = 0;
		letrinha = SBUF0;
		if (letrinha != 8) {
			AddToUARTBuff(letrinha, UARTBuffer, UARTBufferindex);
			Recieved++;
			if (letrinha == 13) {
				Read = Recieved;
				Recieved = 0;
				UARTBufferindex = 0;
				TramaCompleta = 1;
			}
		}	else {
			Recieved --;
			UARTBufferindex--;
			if(Recieved < 0)
				Recieved = 0;
			if(UARTBufferindex < 0 || UARTBufferindex == 255)
				UARTBufferindex = 0;
			UARTBuffer[UARTBufferindex] = EMPTY;
		}
		
	}
	if (TI0 == 1) {
		
	}
} 

void DutyChange(signed char change, bit what) {
	if (what)
		DutyCycle100 += change;
	else
		DutyCycle100 = change;
}

void DebugPrint() {
	
			if (Recieved != RecievedAnt && DEBUG) {
				printf("\n");
				for (i = 0; i < 8; i++)
					if(UARTBuffer[i] != 13)
						printf("%c ", UARTBuffer[i]);
					else
						printf("\\n ");
				RecievedAnt = Recieved;
			}
				
}
	
