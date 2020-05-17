#pragma large
#include <REG51F380.H>
#include "uart.h"

#define BUFFERSIZE 16
#define EMPTY 95

extern bit tramaCompleta;

char UARTBuffer[BUFFERSIZE];
char UARTBufferindex = 0;

char StringLen(const char* string) {
  char len = 0;
  while (string[len] != '\0') {
    len++;
  }
  return len;
}

char ler_caracter() {
		char RET = -1;
		if (RI0 == 1)	{
			RET = SBUF0;
			RI0 = 0;
		}
		return RET;
}

void print(char *character, bit var) {
    if(var == 0) {
      char i;
      for(i = 0; i < StringLen(character); i++) {
        SBUF0 = (character[i] == '\n') ? 13 : character[i];
    		while(TI0 == 1)
    			TI0 = 0;
      }
    } else if(var == 1) {
      char digit[3] = {0, 0, 0};
      char i = 2;
      char number = (char)*character;
      while (number > 0) {
       digit[i] = number % 10;
       i--;
       number /= 10;
      }
      for(i = 0; i < 3; i++) {
        SBUF0 = digit[i]+48;
        while(TI0 == 1)
          TI0 = 0;
      }
    }
}

void printBuffer() {
  print(UARTBuffer, 0);
  print("\n", 0);
}

void Add2Buffer(char letra) {
  if(letra == '\b') {
    if (UARTBufferindex > 0) {
      UARTBufferindex--;
      UARTBuffer[UARTBufferindex] = EMPTY;
    }
  }
  else {
    UARTBuffer[UARTBufferindex] = letra;
    UARTBufferindex++;
    UARTBufferindex &= BUFFERSIZE-1;
  }
}

char CheckSerial() { // -1 nada  // 0 normal // 1 enter
		char readed = 0;
		readed = ler_caracter();
    if(readed != -1) {
			readed -= (readed <= 172 && readed >= 97) ? 32 : 0;
      Add2Buffer(readed);
      if(readed == 13)
        tramaCompleta = 1;
      return (readed == 13) ? 1 : 0;
    }
    return -1;
}

void Reset() {
  char i = 0;
  for(i = 0; i < BUFFERSIZE; i++)
    UARTBuffer[i] = EMPTY;
  UARTBufferindex = 0;
}
