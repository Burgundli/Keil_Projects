#include "si_toolchain.h"
#include "C8051F330_defs.h"
#include "stdio.h"

extern void Init_Device(); 
unsigned char c; 
float freq, sysclock; 

unsigned int convert; 

void main(void)
{
	Init_Device(); 

	TI0 = 1; 
	
	switch((OSCICN & 0x03))
	{	
		case 0x00:
			sysclock = 3.0625; 
			break; 
		case 0x01:
			sysclock = 6.125; 
			break; 
		case 0x02:
			sysclock = 12.25; 
			break;
		case 0x03:
			sysclock = 24.5; 
			break;
	}
	
	while(1)
	{
		printf("Vyber frekvenciu AD prevodnika v rozsahu %0.2f - %0.2f MHz (Hodnota v MHz)", (float)(sysclock / 32), (float)sysclock); 
		scanf("%f", &freq);
		if (freq < (sysclock / 32) && freq > sysclock)
		{
			printf("Hodnota zadana je mimo rozsah");
			continue; 
		}
		convert = (int)((sysclock/ freq) - 1); 
		
		ADC0CF = convert << 3; 
		
		printf("Frekvencia SAR registra ADC prevodnika nastavena na hodnotu: %f Hz\n", (float)((sysclock * 1000000) / (convert + 1)));
		
	}

}