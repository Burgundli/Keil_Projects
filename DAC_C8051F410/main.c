#include "si_toolchain.h"
#include "C8051F410_defs.h"
#include "stdio.h"


extern void Init_Device(); 

#define UREF 3.0f
#define KROK (float) UREF / 511 // naozaj 1023 pri merani oproti zemi (GND)
	
unsigned char c, d;
int ui_i;
float f;
unsigned int i;


void main (void)
{
	Init_Device(); 
	TI0 = 1; 
	
	while(1)
	{
		printf("\n1 - Vyber rozsah DA prevodnika, 2 - Nastav prud v mA\n");  
		c = getchar();
		switch(c)
		{
			case '1':
				printf("\nVyber rozsah DA prevodnika:\n1 - 500uA, 2 - 1mA, 3 - 2mA\n");
				c = getchar(); 
				//zistim prud 
				d = IDA0CN & 0x03; 
				i = (IDA0H << 8) | (IDA0L);
			
				//Konvertovanie hodnoty registra na hodnotu prudu
				if (c == '1')
				{
						f = (float)(i * 0.0001221); 
				}
				else if (c == '2')
				{
						f = (float)(i * 0.0002442); 
				}
				else if (c == '3')
				{
						f = (float) (i * 0.0004884); 
				}
				
				if (c == '1') 
				{
					IDA0CN &=~ 0x80; //Vypnutie prevodnika  
					IDA0CN &=~ 0x02;  // Nastavujem bity IDA0OMD[0] do 1 a IDA0OMD[1] do 0 - nastavenie 500 uA rozsahu
					IDA0CN |= 0x01; 
					IDA0CN |= 0x80;  //Zapnutie prevodnika
					
					i = (float)(f * 8192); //konverzia prudu na hodnotu registra

					IDA0L = i & 0xFF; //dolne bity vynasobene maskou a vlozene do dolneho registra
					IDA0H = i >> 8; //horne bity posunute o 8 bitov do prava a ulozene do horneho registra
				}
				else if (c == '2') 
				{
					IDA0CN &=~ 0x80; //Vypnutie prevodnika  
					IDA0CN &=~ 0x01;  // Nastavujem bity IDA0OMD[0] do 0 a IDA0OMD[1] do 1 - nastavenie 1mA rozsahu
					IDA0CN |= 0x02; 
					IDA0CN |= 0x80;  //Zapnutie prevodnika
					
					i = (float)(f * 4096);  //konverzia prudu na hodnotu registra

					IDA0L = i & 0xFF;  //dolne bity vynasobene maskou a vlozene do dolneho registra
					IDA0H = i >> 8; //horne bity posunute o 8 bitov do prava a ulozene do horneho registra
				}
				else if (c == '3') 
				{
					IDA0CN &=~ 0x80; //Vypnutie prevodnika  
					IDA0CN |= 0x03;  //  nastavenie 2 mA rozsahu  oba bity IDA0OMD[0:1] v 1  
					IDA0CN |= 0x80;  //Zapnutie prevodnika
					
					i = (float)(f * 2048); //konverzia prudu na hodnotu registra

					IDA0L = i & 0xFF;  //dolne bity vynasobene maskou a vlozene do dolneho registra
					IDA0H = i >> 8;  //horne bity posunute o 8 bitov do prava a ulozene do horneho registra
				}
				break; 
			case '2':
				c = 0x00; 
				printf("\nZadaj hodnotu v mA\n"); 
				scanf("%f", &f);
				c = IDA0CN & 0x03; // citanie rozsahu prevodnika
			
				switch(c)					
				{
					//0.5 mA
					case 1: 
						i = (float)f * (4096 / 0.5); 
						break; 
					//1 mA
					case 2: 
						i = (float)f * (4096 / 1);  
						break;
					//2 mA
					case 3:
						i = (float)f * (4096 / 2); 
						break;
				
				}
				if (i > 4095) // kontrola ci zadana hodnota sedi v rozsahu
				{
					printf("\nZadana hodnota je mimi rozsahu!\n "); 
					break; 
				}
				
				d = IDA0CN & 0x03;
				if (d == 1)
					printf("Nastavena hodnota je %.01f mA\n", (float)( i * 0.0001221)); //chyba zle vypisuje v print ale dobre zadáva
				else if  (d == 2)
					printf("Nastavena hodnota je %.01f mA\n", (float)(i * 0.0002442));  //chyba zle vypisuje v print ale dobre zadáva
				else if (d == 3)
					printf("Nastavena hodnota je %.01f mA\n", (float)(i * 0.0004884)); //chyba zle vypisuje v print ale dobre zadáva
				
				IDA0L = i & 0xFF; 
				IDA0H = i >> 8; 
				break; 
		}

	}

}