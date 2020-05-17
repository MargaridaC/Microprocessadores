#include <REG51F380.H>
#include <stdio.h>
#include "uart.h"
#include "init.h"

char index = 0;

void main (void)
{
		Init();
		while(1)
		{
			index = CheckSerial(index);
		}
}
