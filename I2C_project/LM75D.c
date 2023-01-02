#include "C8051F330.h"

sbit SDA = P0^2;
sbit SCL = P0^3;

void DelayUs (unsigned int us)    // oneskorenie x us
{
TMR2L = 0xF3;				//nastavenie pocitocnej hodnoty
TMR2H = 0xFF;
	TR2 = 1;			//zapnutie casovaca
	TF2H = 0;			// vypnutie priznaku prerusenia
	while (us)			// opakuj dokedy us je vacsie ako 1
	{
		while(!TF2H);			//cakame dokedy je v nule
		TF2H = 0;					// vypnutie priznaku prerusenia
		us--;
	}
}

void starti2c(void)
{
	SDA =1; SCL = 1;
	DelayUs(2);
	SDA = 0;
	DelayUs(2);
	SCL = 0;
}															//start i2c

void poslibyte(unsigned char bytew)  //posli byte
{
	char iii;
	for(iii = 0; iii < 8; iii++)
	{
		SDA = bytew & 0x80;
		DelayUs(2);
		SCL = 1;
		DelayUs(2);
		SCL = 0;
		bytew <<= 1;   // posun o jeden bit
	}
}

void posliack (void)		//potvrdzovaci bit ak citam zo zariadenia
{
	SDA = 0;
	DelayUs (2);
	SCL = 1;
	DelayUs (2);
	SCL = 0;
	DelayUs (2);
	SDA = 1;
}

bit prijmiack(void)   //citam potvrdzovaci bit  ak posielam do zariadenia
{
	bit OK = 0;
	SDA = 1;
	DelayUs (2);
	SCL = 1;
	DelayUs (2);
	OK = ~SDA;   // nulou odpoveda zariadenie, preto negujeme odpoved na jednotku
	SCL = 0;
	DelayUs (2);
	return OK;
}

unsigned char prijmibyte(void)
{
	unsigned char iii, Din;
	Din = 0x00;
	SDA = 1;
	DelayUs (2);																	//prijmi byte i2c
	for  (iii = 0; iii < 8; iii++)
	{
		Din <<= 1;
		SCL = 1;
		DelayUs (2);
		Din |= SDA;
		SCL = 0;
		DelayUs(2);
	}
	return(Din);
}

void stopi2c(void)
{
	SDA = SCL = 0;
	DelayUs (2);
	SCL = 1;
	DelayUs (2);
	SDA = 1;
}

float Citaj_Teplotu(void)
{
	char bajt;
	float teplota;
	starti2c();
	poslibyte(0x90);		// posiela adresu teplomera LM75D - zapis
	prijmiack();				// Prijmeme odpoved
	poslibyte(0x00);		// Adresa teploty v cipe
	prijmiack();
	starti2c();					// opakovany start pre citanie
	poslibyte(0x91);		// posiela adresu teplomera LM75D - citanie
	prijmiack();				// prijmeme odpoved
	bajt = prijmibyte();   //Nacitanie bajtu z LM75D
	teplota = bajt;
	posliack();
	bajt = prijmibyte();		//Nacitanie bajtu z LM75D
	posliack();
	stopi2c();
	teplota += 0.5;
	
	return teplota;
}