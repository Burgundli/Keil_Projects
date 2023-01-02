#include "stdio.h"
#include "si_toolchain.h"
#include "C8051F330_defs.h"

sbit LED = P0^0; 

#define INBF_SIZE  200

xdata unsigned char buffer[100], input_buffer[INBF_SIZE]; 
int i, j, buffer_size, vz_i, k, l; 
bit new_row; 

extern Init_Device(); 

void main(void)
{
	Init_Device(); 
	vz_i = 0; 
	
	while(1)
	{
		if (i > 99)
		{
			buffer_size = sprintf(&buffer[0], "Ahoj svet!\n");
			j = 0;			
			SBUF0 = buffer[j++]; 
			i = 0;
		}
		if (new_row)
		{
			buffer_size = vz_i; 
			for(k = 0; k < vz_i; k++)
			{
				buffer[k] = input_buffer[k++]; 
			}
			l = 0; 
			SBUF0 = buffer[l++]; 
			buffer_size = vz_i; 
			new_row = 0; 
		}
	}
}

void UART0_ISR(void) interrupt 4 
{
	if (RI0)
	{
		if (vz_i < INBF_SIZE)
		{
			input_buffer[vz_i] = SBUF0; 
			if (input_buffer[vz_i] == 0x0D)
			{
				new_row = 1; 
			}
			vz_i++; 
		}
		RI0 = 0; 
	}
	if (TI0)
	{
		if (j < buffer_size)
		{	
			SBUF0 = buffer[j++]; 
		}
		TI0 = 0; 
	}
}

void TMR3_ISR(void) interrupt 14
{
	LED = ~LED; 
	i++; 
	TMR3CN &=~ 0x80;  

}