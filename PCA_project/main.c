
#include "stdio.h"
#include "si_toolchain.h"
#include "C8051F330_defs.h"

extern void Init_Device(); 
unsigned char c; 
float f; 
unsigned ui_i; 

#define SYSCLK   	12250000
#define CLCKDIV 	4
#define FPCA 			(float) SYSCLK/CLCKDIV

void main(void)
{
	Init_Device(); 
	
	TI0 = 1; 
	
	printf("Priklad PCA \n"); 
	
	while(1)
	{
		do
		{
			printf("Vyber modul:\n"); 
			printf("1 - frekvencny vystup");
			printf("2 - PWM vystup 8-bit");
			printf("3 - PWM vystup 16-bit\n");
			c = getchar(); 
			
		} 
		while((c != '1') &&(c !='2') && (c != '3'));

		switch (c)
		{
			case '1':
				printf("\nZadaj ziadanu frekvenciu\n"); 
				scanf("%f", &f); 
				f = FPCA / ( 2*f );
				PCA0CPH0 = f; 
				f = (float) FPCA / (2* PCA0CPH0);
				printf("Nastavena hodnota frekvencie je %0.1f Hz\n", f); 
				break; 
			case '2':
				printf("\nZadaj ziadany cinitel plnenia v %%\n");
				scanf("%f", &f); 
				f = (float) 256 - (2.56f * f);
				PCA0CPH0 = f; 
				f = (float) (256 - PCA0CPH0) /2.56f;
				printf("Nastaveny cinitel plnenia je %0.3f v %%\n", f); 
				break;
			case '3':
				printf("\nZadaj ziadany cinitel plnenia v %%\n");
				scanf("%f", &f); 
				f = (float) 65536 - (65.536f * f);
				ui_i = f; 
				PCA0CPH2 = ui_i & 0xFF; 
				PCA0CPH2 = ui_i >> 8; 
				f = (float) (65536 - PCA0CPH2) / 65.536f; 
				printf("Nastaveny cinitel plnenia je %0.3f v %%\n", f); 
				break; 
		}
	}
	
}