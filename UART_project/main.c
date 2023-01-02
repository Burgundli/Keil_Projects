#include "C8051F330.h"
#include "stdio.h"
#include "string.h"

extern void Init_Device(); 

unsigned char c;
unsigned int i,a,b;
float f; 

xdata unsigned char meno[20], priezvisko[20], zasobnik[100];

void main(void)
{
	Init_Device(); 
	TI0 = 1;  //Indikacný bit prerušenia pre vysielanie UART
	
	printf("Hello World\n"); 
	
	while(1)
	{
		/*
		printf("Zadaj a:\n"); 
		scanf("%u", &a); 
		printf("Zadaj b:\n"); 
		scanf("%u", &b); 
		do
		{
			printf("Zadaj operator:\n");
			scanf("%c",  &c);
			
		}
		while((c != '*') && (c != '-') && (c != '+') && (c != '/') && (c != '%')); 
		
		switch (c)
		{
			case '*':
				i = a * b; 
				break; 
			case '+':
				i = a + b;
				break; 
			case '-':
				i = a - b;
				break; 
			case '/':
				i = a / b;
				break;
			case '%':
				i = a % b;
				break;
			
		}
		printf("%u %c %u = %u\n", a, c, b, i); 
		*/
		printf("Zadaj meno:\n"); 
		scanf("%s", &meno[0]); 
		printf("Zadaj priezvisko:\n"); 
		scanf("%s", &priezvisko[0]); 
		printf("Zadaj vek:\n"); 
		scanf("%bu", &c); 
		
		strcpy(&zasobnik[0], meno);
		strcat(&zasobnik, " "); 
		strcat(&zasobnik, priezvisko); 
		strcat(&zasobnik, ","); 
		i = strlen(zasobnik); 
		sprintf(&zasobnik[i], "%bu rokor\n", c); 
		
		printf(zasobnik); 
	}
	
}