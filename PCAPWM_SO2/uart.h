#ifndef __UART__
#define __UART__

extern void Reset();

extern char StringCompare(const char str1[8],const  char str2[8]);

extern void ApplyFunction();

extern void AddToUARTBuff(char add);

extern void ReadUARTBuff();

extern void DutyChange(char change, bit what);

extern void encode();

extern void DutyCheck();

extern void DebugPrint();

extern void ConvertToCapital(char *str);

extern void imprime(char st[8]);

#endif
