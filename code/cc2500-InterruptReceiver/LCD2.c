/*********************************************************************
 *
 *      Example User Program to be used with resident Bootloader
 *
 *********************************************************************
 * FileName:        LCD2.c
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC18
 * Compiler:        C18 2.30.01+
 * Company:         Microchip Technology, Inc.
 *

 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Sanchit Arora
 ********************************************************************/

/** I N C L U D E S **********************************************************/

#include <p18cxxx.h>
#include <delays.h>                 //for delay libraries
#include "lcd.h"

/** V E C T O R  R E M A P P I N G *******************************************/
#pragma code

void ClearScreen(void)
{
	WriteCmd(0b00000001);
	Delay_ms(30);
}	

void ResetCursor(void)
{
	WriteCmd(0b00000011);
	Delay_ms(30);
}	

void MoveToDisplay(unsigned char a)
{
	unsigned char b = a | 0b10000000;
	WriteCmd(b);
	Delay_ms(30);
}

void ShiftDisplayRight(int i)
{
	while(i>0)
	{
		WriteCmd(0b00011100);
		Delay_ms(30);
		i--;
	}
}

void ShiftDisplayLeft(int i)
{
	while(i>0)
	{
		WriteCmd(0b00011000);
		Delay_ms(30);
		i--;
	}
}


void WriteInt(int i,int l)
{
	int len=l;
	if(i<0)
	{
		char a[10];
		i=-i;
		while(l>0)
		{
			a[l]=(i%10)+48;
			i=i/10;
			l--;
		}
		a[0]='-';
		WriteStringL(a,len+1);
	}
	else
	{
		char a[10];
		a[l]=' ';
		while(l>0)
		{
			a[l-1]=(i%10)+48;
			i=i/10;
			l--;
		}
		WriteStringL(a,len+1);
	}
}


void WriteLong(long i,int l)
{
	int len=l;
	if(i<0)
	{
		char a[15];
		i=-i;
		while(l>0)
		{
			a[l]=(i%10)+48;
			i=i/10;
			l--;
		}
		a[0]='-';
		WriteStringL(a,len+1);
	}
	else
	{
		char a[15];
		a[l]=' ';
		while(l>0)
		{
			a[l-1]=(i%10)+48;
			i=i/10;
			l--;
		}
		WriteString(a);
	}
}


void WriteULong(unsigned long i,int l)
{
	int len=l;
	char a[15];
	while(l>0)
	{
		a[l-1]=(i%10)+48;
		i=i/10;
		l--;
	}
	WriteStringL(a,len);
}

void WriteData(unsigned char a)
{
	unsigned char i_mask;
	DAT5 =0; DAT4 = 0; DAT6 = 0; DAT7=0;
	
	E = 1;
	i_mask = 0b10000000;
	
	if ((i_mask & a) == i_mask ) {
		DAT7 = 1;
	}
	else {  DAT7 = 0; }
	i_mask = i_mask >> 1;

	if ((i_mask & a) == i_mask ) {
		DAT6 = 1;
	}
	else {  DAT6 = 0; }
	i_mask = i_mask >> 1;

	if ((i_mask & a) == i_mask ) {
		DAT5 = 1;
	}
	else {  DAT5 = 0; }
	i_mask = i_mask >> 1;

	if ((i_mask & a) == i_mask ) {
		DAT4 = 1;
	}
	else {  DAT4 = 0; }
	i_mask = i_mask >> 1;

	// completed sending one nybble
	Delay_ms(1);
	
	E = 0;
	
	Delay_ms(1);
	E = 1;

	if ((i_mask & a) == i_mask ) {
		DAT7 = 1;
	}
	else {  DAT7 = 0; }
	i_mask = i_mask >> 1;

	if ((i_mask & a) == i_mask ) {
		DAT6 = 1;
	}
	else {  DAT6 = 0; }
	i_mask = i_mask >> 1;

	if ((i_mask & a) == i_mask ) {
		DAT5 = 1;
	}
	else {  DAT5 = 0; }
	i_mask = i_mask >> 1;

	if ((i_mask & a) == i_mask ) {
		DAT4 = 1;
	}
	else {  DAT4 = 0; }

	//Delay1TCY();
	//Delay1TCY();
    Delay_ms(1);
	E = 0;
	DAT5 =0; DAT4 = 0; DAT6 = 0; DAT7=0;

}


void WriteNybble( unsigned char a) 
{
	unsigned char i_mask;
	DAT5 =0; DAT4 = 0; DAT6 = 0; DAT7=0;
	RS = 0;
	E = 1;
	i_mask = 0b10000000;
	
	if ((i_mask & a) == i_mask ) {
		DAT7 = 1;
	}
	else {  DAT7 = 0; }
	i_mask = i_mask >> 1;

	if ((i_mask & a) == i_mask ) {
		DAT6 = 1;
	}
	else {  DAT6 = 0; }
	i_mask = i_mask >> 1;

	if ((i_mask & a) == i_mask ) {
		DAT5 = 1;
	}
	else {  DAT5 = 0; }
	i_mask = i_mask >> 1;

	if ((i_mask & a) == i_mask ) {
		DAT4 = 1;
	}
	else {  DAT4 = 0; }
	
	Delay_ms(10);	
	E = 0;
	
	DAT5 =0; DAT4 = 0; DAT6 = 0; DAT7=0;
}

void WriteCmd(unsigned char a)
{
	RS = 0;
	WriteData(a);
}

void WriteChar(unsigned char a)
{
	RS = 1;
	WriteData(a);
}

void WriteStringL(char* tolcd,int l)
{
	while(l>0)
	{
		WriteChar(*tolcd);
		Delay_ms(1);
		tolcd++;
		l--;
	}
}

void WriteString(char* tolcd)
{
	int i=0;
	while(tolcd[i]!='\0')
	{
		WriteChar(tolcd[i]);
		Delay_ms(1);
		i++;
	}
}

void init_LCD4(){
	
	Delay_ms(300); // delay of 100 ms after Vcc

//	BackL=1;

	WriteNybble(0b00100000);      //0x20
	Delay_ms(30);
	
	WriteCmd(0b00101000);        //0x28
	Delay_ms(30);

	WriteCmd(0b00001000);       //0x08
	Delay_ms(30);
	
	WriteCmd(0b00000001);        //0x01  clearing
	Delay_ms(30);

	WriteCmd(0b00000110);       //0x03
	Delay_ms(30);

	WriteCmd(0b00001111);       //0x0E
	Delay_ms(30);
	//BackL = 0;
}

void OpenLCD(void)
{
  //	TRISDbits.TRISD3=0;			// BackL
	TRISBbits.TRISB7=0;			// RS
	//TRISBbits.TRISB6=0;			// RW
	TRISBbits.TRISB5=0;			// E
	TRISDbits.TRISD7=0;			// DAT4
	TRISDbits.TRISD6=0;			// DAT5
	TRISDbits.TRISD5=0;			// DAT6
	TRISDbits.TRISD4=0;			// DAT7
}

