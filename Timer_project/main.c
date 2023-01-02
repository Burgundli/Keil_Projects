#include "C8051F330.h"

extern void Init_Device(); 

sbit LED = P0^0;  //Vyvod P0 budeme nazyvat LED
sbit LED2 = P0^1;  //Vyvod P0 budeme nazyvat LED

void main(void)
{
	Init_Device(); 
	
	while(1)
	{
		/*
		while(!TF0);
		TL0 = 0x4B;
		TH0 = 0xA0; 		// pri zápise THO
		LED = ~LED; 
		TF0 = 0;  			//Mazanie indikatora pretecenia
	*/
		if(TF2H)
		{
			LED = ~LED; 
			TF2H = 0; 
		}
		if(TF2L)
		{
			LED2 = ~LED2; 
			TF2L= 0; 
		}
		
	 
		
	}
	
}