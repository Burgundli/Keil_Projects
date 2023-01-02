#include "stdio.h"
#include "si_toolchain.h"
#include "C8051F330_defs.h"

extern void Init_Device(); 
#define UREF 3.0f 
#define KROK (float) UREF / 511 //1023 pri meraní od GND
	
unsigned char c,d; 
int ui_i; 
float f; 
unsigned int i; 

void main(void)
{
	Init_Device(); 
	TI0 = 1; 
	
	printf("AD/DA prevodníky príklad\n"); 
	while(1)
	{
		printf("\nVyber prevodník:\n");
		printf("1 - AD , 2 - DA"); 
		c = getchar(); 
		if (c == '1')
		{
			printf("\nVyber vstup:\n1-P1.0, 2- (P1.1-P1.2), t -teplota\n");
			c = getchar(); 
			switch(c)
			{
				case '1':
					AMX0N = 0x11; //GND bude zaporny vstup; 
					AMX0P = 0x08; // Vstup bude P1.0
					AD0INT = 0; 
					AD0BUSY = 1; 	//Spustenie meranias
					while(!AD0INT); //Cakam kym AD0INT nebude v log 1
					ui_i = (unsigned int) ADC0H << 8 | ADC0L; 
					f = (float) ui_i * KROK; //krok sa pocita UREF /1023 - v simulácií 511
					printf("Napatie na P1.0 je %.03f V\n", f); 
					break; 
				case '2':
					AMX0N = 0x0A; //P1.2 bude zaporny vstup; 
					AMX0P = 0x09; //Kladny vstup bude P1.1
					AD0INT = 0; 
					AD0BUSY = 1; 
					while(!AD0INT); //Cakam kym AD0INT nebude v log 1
					ui_i = (unsigned int) ADC0H << 8 | ADC0L; 
					f = (float) ui_i * KROK; //krok sa pocita UREF /1023 - v simulácií 511
					printf("Rozdiel napatí P1.1 a P1.2 je %.03f V\n", f); 
					break; 
				case 't':
					AMX0N = 0x11; //GND bude zaporny vstup; 
					AMX0P = 0x09; //Kladny vstup bude teplotny snimac
					AD0INT = 0; 
					AD0BUSY = 1; 
					while(!AD0INT); //Cakam kym AD0INT nebude v log 1
					ui_i = (unsigned int) ADC0H << 8 | ADC0L; 
					f = (float) ui_i * KROK *1000; //krok sa pocita UREF /1023 - v simulácií 511
					f -= 776; 
					f/= 2.86; 
					printf("Teplota je %.01f C\n", f); 
					break; 
				}
			}
			else if (c == '2')
			{
				printf("\n1 - Vyber rozsah DA prevodnika, 2 - Nastav prud v uA\n"); 
				c = getchar();
				switch(c)
				{
					case '1':
						printf("\nVyber rozsah DA prevodnika:\n1 - 500uA, 2 - 1mA, 3 - 2mA\n"); 
						c = getchar(); 
					//zistim prud 
						d = IDA0CN & 0x03; 
						i = (IDA0H << 2) | (IDA0L >> 6);
						if (d == 0) f = (float) 1 /2.046; 
						else if (d == 1) f = (float) 1 / 1.023; 
						else f = (float) i *1.95503; 
						if (c == '1')
						{
							IDA0CN &=~ 0x80; //Vypnutie prevodnika  
							IDA0CN &=~ 0x03; //Nulujem bity - nastavenie 500uA
							IDA0CN |= 0x80;  //Zapnutie prevodnika
							
							f = (float) f * 2.046; 
							i = f; 
							i <<= 6; 
							IDA0L = i & 0xFF; //Zapisom do IDA0L sa vystupny prud nemeni  
							IDA0H = i >> 8; //Zapisom do IDA0H sa aktualizuje cely vystup
						}
						else if (c == '2')
						{
							IDA0CN &=~ 0x80; //Vypnutie prevodnika  
							IDA0CN &=~ 0x02; //Nulujem bity - nastavenie 1mA
							IDA0CN |= 0x01; 
							IDA0CN |= 0x80;  //Zapnutie prevodnika
							
							f = (float) f * 1.023; 
							i = f; 
							i <<= 6; 
							IDA0L = i & 0xFF; //Zapisom do IDA0L sa vystupny prud nemeni  
							IDA0H = i >> 8; //Zapisom do IDA0H sa aktualizuje cely vystup
						}
						else if (c == '3')
						{
							IDA0CN &=~ 0x80; //Vypnutie prevodnika  
							IDA0CN &=~ 0x02; //Nulujem bity - nastavenie 
							IDA0CN |= 0x80;  //Zapnutie prevodnika
							
							f = (float) f * 1.95503; 
							i = f; 
							i <<= 6; 
							IDA0L = i & 0xFF; //Zapisom do IDA0L sa vystupny prud nemeni  
							IDA0H = i >> 8; //Zapisom do IDA0H sa aktualizuje cely vystup
						}
						break; 
					case '2':
						printf("\nZadaj hodnotu v uA\n"); 
						scanf("%f", &f);
						c = IDA0CN & 0x03;
						if (c == 0) f = (float) f * 2.046; 
						else if (c == 1) f = (float) f * 1.023;
						else f = (float) f / 1.95503; 
						i = f; 
						d = IDA0CN & 0x03; 
						if (d == 0)
							printf("Nastavena hodnota je %0.1f uA\n", (float) i/ 2.046);
						else if (d == 1)
							printf("Nastavena hodnota je %0.1f uA\n", (float) i/ 1.023);
						else 
							printf("Nastavena hodnota je %0.1f uA\n", (float) i/ 1.95503 * i);
						i <<= 6;
						IDA0L = i & 0xFF; 
						IDA0H = i >> 8; 
						break; 
				
				}
			
			}
		
		}
	
}