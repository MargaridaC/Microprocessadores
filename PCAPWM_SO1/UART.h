#ifndef __UART__
#define __UART__

extern void Reset();

extern char StringCompare(char str1[8], char str2[8]);

extern void ApplyFunction(char DutyCycle, char UARTFunction[8]);

extern void AddToUARTBuff(char add, char UARTBuffer[8], char UARTBufferindex);

extern void ReadUARTBuffbit(TramaCompleta, char UARTBuffer[8], bit flagTrama, char Read, char UARTFunction[8]);

extern void DutyChange(signed char change, bit what)

#endif