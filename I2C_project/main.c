#include "C8051F330.h"
#include "stdio.h"

sbit SDA = P0^2;
sbit SCL = P0^3;

extern void Init_Device();
extern float Citaj_Teplotu(void);
extern void DelayUs(unsigned int us);
unsigned int i;

void main (void) 
{
	SDA = SCL = 1;				//Inicializacia komunikacnych vodicov
	Init_Device();
	TI0 = 1;
	
	while(1)    
	{
		printf("%.01f st C\n", Citaj_Teplotu());
		for (i= 0; i < 1000; i++)
		DelayUs(1000);
	}
}

