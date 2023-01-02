#include "si_toolchain.h"
#include "C8051F330_defs.h"
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
	
	i = RSTSRC; 
	
	if (0x01)
		printf("Nulovanie z vyvodu\n"); 
	if (0x02)
		printf("Nulovanie z start\n"); 
	if (0x04)
		printf("Nulovanie z detekcia hodin\n"); 
	if (0x08)
		printf("Nulovanie z watchdog\n"); 
	if (0x10)
		printf("Nulovanie z komparatora\n"); 
	if (0x20)
		printf("Nulovanie z vyvodu\n"); 
	if (0x40)
		printf("Nulovanie z chyby flash\n"); 
	
	
	while(1)    
	{
		PCA0CPH2 = 0; 
		printf("%.01f st C\n", Citaj_Teplotu());
		for (i= 0; i < 1000; i++)
		DelayUs(1000);
	}
}

