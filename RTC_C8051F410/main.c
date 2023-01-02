#include "si_toolchain.h"
#include "C8051F410_defs.h"
#include "stdio.h"

extern void Init_Device(); 

unsigned char num_monthD[12] = {31,28,31,30,31,30,31,31,30,31,30,31}; 
bit leap_flag, alarm_flag; 

struct SYS_TIME
{
	unsigned char seconds; 
	unsigned char minutes; 
	unsigned char hours; 
	unsigned char day;
	unsigned char month; 
	unsigned int year; 
}; 

struct SYS_TIME systemTime;
unsigned long timer; 

unsigned char time[6], month, c, alarm_EN; 
unsigned short days; 

unsigned int i, leap_days, dayOfYear, y, m; 


void main (void)
{
	Init_Device();  
	
	TI0 = 1; 
	printf("Unlocking RTC interface....\n"); 
	
	if (RTC0KEY == 0x00)
	{
		RTC0KEY = 0xA5;
		RTC0KEY = 0xF1;
	}
	
	if (RTC0KEY == 0x02)
	{
		printf("RTC interface unlocked...\n"); 
	}
	if (RTC0KEY == 0x03)
	{
		printf("RTC module disabled until next restart... (Wrong key codes!)\n");
		return; 
	}
	
	//smaRTClock setup
	
	//R/W operation 1 Sysclock + Select reg RTC0XCN
	RTC0ADR = (0x10) | (0x07); 
	//XMODE Self-Oscilate Mode
	RTC0DAT = 0x40; 
	while((RTC0ADR & 0x80) == 0x80);
	
	//BIASX2 40 kHz
	RTC0DAT = 0x20; 
	while((RTC0ADR & 0x80) == 0x80); 
	
	//enable timer incementation
	RTC0ADR = (0x10) | (0x06);
	RTC0DAT  = 0x10;
	while((RTC0ADR & 0x80) == 0x80); 	
	
	//set starting date of RTC timer 
	RTC0ADR = 0x00 | 0x10 | 0x80;
	

	RTC0DAT = 0x00;
	while((RTC0ADR & 0x80) == 0x80); 		
	RTC0DAT = 0x00; 
	while((RTC0ADR & 0x80) == 0x80); 	
	RTC0DAT = 0xD0;
	while((RTC0ADR & 0x80) == 0x80); 		
	RTC0DAT = 0x9E;
	while((RTC0ADR & 0x80) == 0x80); 		
	RTC0DAT = 0x87;
	while((RTC0ADR & 0x80) == 0x80); 		
	RTC0DAT = 0x63; 
	while((RTC0ADR & 0x80) == 0x80); 
	
	RTC0ADR = (0x10) | (0x06);
	RTC0DAT = (0x02) | (0x10);
	while((RTC0DAT & 0x02) == 0x02 && ((RTC0ADR & 0x80) == 0x80));
	
	while(1)
	{
		printf("\nPress : 1 -> Get current date and time 2 -> Set Alarm\n"); 
		c = getchar();
		
		if (c == '1')
		{
				//Setting RTC0CAP and inddirect value read with set BUSY bit
				RTC0ADR = (0x10) | (0x06) | (0x80);
			
				while((RTC0ADR & 0x80) == 0x80);
			
				alarm_EN = RTC0DAT & 0xFF;
			
				if ((alarm_EN & 0x08) == 0x08)
				{
					RTC0ADR = (0x10) | (0x06);
					RTC0DAT = (0x01) | (0x10) | (0x08);
				}
				else
				{
					RTC0ADR = (0x10) | (0x06);
					RTC0DAT = (0x01) | (0x10);
				}		
			
				while((RTC0DAT & 0x01) == 0x01 && ((RTC0ADR & 0x80) == 0x80));
		
				//set busy bit and autoread from CAPTUREn registers
				RTC0ADR = 0x00 | 0x10 | 0x80 | 0x40;
						
				while((RTC0ADR & 0x80) == 0x80);
				
				time[0] = RTC0DAT;
				while((RTC0ADR & 0x80) == 0x80);
				
				time[1] = RTC0DAT;
				while((RTC0ADR & 0x80) == 0x80);
				
				time[2] = RTC0DAT;
				while((RTC0ADR & 0x80) == 0x80);
				
				time[3] = RTC0DAT;
				while((RTC0ADR & 0x80) == 0x80);
				
				time[4] = RTC0DAT;
				while((RTC0ADR & 0x80) == 0x80);
				
				time[5] = RTC0DAT;
				while((RTC0ADR & 0x80) == 0x80);
				
				//join values from CAPTUREn registers 
				timer = time[5]; 
				timer <<= 8; 
				timer |= time[4];
				timer <<= 8;
				timer |= time[3]; 
				timer <<= 8; 
				timer |= time[2];
				
				//set offset from timezone (GTM+1) -> 1h = 3600s
				timer += 3600; 
				
				//calculate time  
				systemTime.seconds = timer % 60; 
				timer /= 60; 
				systemTime.minutes = timer % 60; 
				timer /= 60;
				systemTime.hours = timer % 24; 
				timer /= 24;
				
				//reset values
				days = 0; 
				month = 0; 
				leap_days = 0;
				i = 0; 		
				
				//calculate accurate year based on leap days
				systemTime.year = 1970 + (timer / 365);
				
				for(i = 1972; i < systemTime.year; i += 4) leap_days++; 
				
				systemTime.year = 1970 + ((timer  - leap_days) / 365); 
				
				dayOfYear = ((timer - leap_days) % 365) + 1;
				
				//check if current year is leap and set days of February 
				if((((systemTime.year % 4) == 0) && ((systemTime.year % 100) != 0)) || ((systemTime.year % 400) == 0))
				{
					num_monthD[1] = 29; 
					leap_flag = 1; 
				}
				else
				{
					num_monthD[1] = 28; 
					leap_flag = 0; 
				}
			
				//calcualte month and days 
				for(month = 0; month <= 11; month++)
				{
					if (dayOfYear <= days)
					{
						break; 
					}
					days = days + num_monthD[month]; 
				}
				
				days = days - num_monthD[month - 1]; // -1 offset for breaking in if condition dayOfYear <= days
				systemTime.month = month; 
				systemTime.day = dayOfYear - days; 
				printf("\n"); 
				printf("%02bu/%02bu/%04u %02bu:%02bu:%02bu \n", systemTime.day, systemTime.month, systemTime.year, systemTime.hours, systemTime.minutes, systemTime.seconds); 
		}
		else if (c == '2')
		{
				printf("\nSet alarm for (format dd/mm/yyyy hh:mm:ss) :"); 
				scanf("%02bu/%02bu/%04u %02bu:%02bu:%02bu", &systemTime.day, &systemTime.month, &systemTime.year, &systemTime.hours, &systemTime.minutes, &systemTime.seconds);
				/*
				systemTime.day = 30; 
				systemTime.month = 11; 
				systemTime.year = 2022; 
				systemTime.hours = 19; 
				systemTime.minutes = 20; 
				systemTime.seconds = 15; 
				*/
				m = 0; 
				y = 0; 
				m = systemTime.month; 
				y = systemTime.year; 
				
				//January and February are counted as 13 and 14 of previous year
				if (systemTime.month <= 2)
				{
					m += 12; 
					y -= 1;  
				}
				
				//years to days
				timer = (365 * (long)y);  
				timer += ((long)y / 4);  
				timer -= ((long)y /100); 
				timer += ((long)y / 400);
				//months to days 
				timer += (30 * (long)m); 
				timer += (3 * ((long)m + 1) / 5); 
				timer += systemTime.day;
				//1/1/1970
				timer -= 719561;
				//days to seconds
				timer *= 86400; 
				
				timer+= (3600 * (long)systemTime.hours) + (60 * (long)systemTime.minutes) + (long)systemTime.seconds; 
				
				//set offset from timezone (GTM+1) -> 1h = 3600s
				timer-= 3600; 
				
				time[2] = (char)(timer & 0xFF);
				timer >>= 8; 
				time[3] = (char)(timer & 0xFF); 
				timer >>= 8;
				time[4] = (char)(timer & 0xFF); 				
				timer >>= 8;
				time[5] = (char)(timer & 0xFF); 	
				
				//Select RTC0CN
				RTC0ADR = (0x10) | (0x06);
			
				//Disable Alarm with RTC0AEN bit 
				RTC0DAT &=~ (0x08); 
				while((RTC0ADR & 0x80) == 0x80);
			
				RTC0ADR = 0x08 | 0x10;
				
				RTC0DAT = 0x00; 
				RTC0ADR |= 0x80; 
				while((RTC0ADR & 0x80) == 0x80);
				
				RTC0DAT = 0x00; 
				RTC0ADR |= 0x80; 
				while((RTC0ADR & 0x80) == 0x80);
				
				RTC0DAT = time[2]; 
				RTC0ADR |= 0x80; 
				while((RTC0ADR & 0x80) == 0x80);
				
				RTC0DAT = time[3]; 
				RTC0ADR |= 0x80; 
				while((RTC0ADR & 0x80) == 0x80);
				
				RTC0DAT = time[4]; 
				RTC0ADR |= 0x80; 
				while((RTC0ADR & 0x80) == 0x80);
				
				RTC0DAT = time[5]; 
				RTC0ADR |= 0x80; 
				while((RTC0ADR & 0x80) == 0x80);
				
				//Select RTC0CN
				RTC0ADR = (0x10) | (0x06);
			
				//Enable Alarm with RTC0AEN bit 
				RTC0DAT |= (0x08); 
				while((RTC0ADR & 0x80) == 0x80);
				
				printf("\nAlarm set for : %02bu/%02bu/%04u %02bu:%02bu:%02bu \n", systemTime.day, systemTime.month, systemTime.year, systemTime.hours, systemTime.minutes, systemTime.seconds);
				
		}

	}	
	

}

void ALRM_ISR(void) interrupt 8 
{
	//Select RTC0CN and clear ALRM bit 
	RTC0ADR = (0x10) | (0x06);
	RTC0DAT  &=~ 0x04;
	while((RTC0ADR & 0x80) == 0x80); 	
	
	//Disable Alarm with RTC0AEN bit 
	RTC0DAT &=~ (0x08); 
	while((RTC0ADR & 0x80) == 0x80);
	
	//Clear ALARMn registers to avoid constant interrupts
	RTC0ADR = 0x08 | 0x10;
	
	RTC0DAT = 0x00; 
	RTC0ADR |= 0x80; 
	while((RTC0ADR & 0x80) == 0x80);
	
	RTC0DAT = 0x00; 
	RTC0ADR |= 0x80; 
	while((RTC0ADR & 0x80) == 0x80);
	
	RTC0DAT = 0x00; 
	RTC0ADR |= 0x80; 
	while((RTC0ADR & 0x80) == 0x80);
	
	RTC0DAT = 0x00; 
	RTC0ADR |= 0x80; 
	while((RTC0ADR & 0x80) == 0x80);
	
	RTC0DAT = 0x00; 
	RTC0ADR |= 0x80; 
	while((RTC0ADR & 0x80) == 0x80);
	
	RTC0DAT = 0x00; 
	RTC0ADR |= 0x80; 
	while((RTC0ADR & 0x80) == 0x80);
	
	alarm_flag = 1; 
	
	printf("\n---------------GOOD MORNING PEOPLE----------------\n");
	
}

