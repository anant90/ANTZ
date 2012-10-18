/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>
#include <delays.h>
#include <usart.h>
#include <timers.h>
#include <adc.h>
#include "lcd.h"
/** V A R I A B L E S ********************************************************/
#pragma udata
/** V E C T O R  R E M A P P I N G *******************************************/
extern void _startup (void);        // See c018i.c in your C18 compiler dir
#pragma code _RESET_INTERRUPT_VECTOR = 0x000800
void _reset (void)
{
    _asm goto _startup _endasm
}
#pragma code

#pragma code _HIGH_INTERRUPT_VECTOR = 0x000808
void _high_ISR (void)
{
	_asm
    	goto InterruptHandlerHigh //jump to interrupt routine
  	_endasm
}

#pragma code _LOW_INTERRUPT_VECTOR = 0x000818
void _low_ISR (void)
{
    ;
}
#pragma code
/** D E C L A R A T I O N S **************************************************/


/**interrrupt routine**********************************************************/
#pragma interrupt InterruptHandlerHigh
void InterruptHandlerHigh ()
{	int i;
	char c;
	if(PIR1bits.RCIF){
	/*	for(i = 0;i<10;i++){
			led_toggle();
			Delay_ms(100);
		}	*/
		//ClearScreen();
		c = RCREG;
		WriteChar(c);
		INTCONbits.GIEH = 1;
	}		
}	

/*******************to give delay of mili seconds******************/
void Delay_ms (int i)
{ 
	int j=0;                 //for 48 Mhz clock
  	// Delay of 1s
	// Cycles = (TimeDelay * Fosc) / 4
	// Cycles = (10s * 48MHz) / 4
	// Cycles = 120,000,000 = 6*200*10000
	while(j<i)
	{ 
	 	Delay10TCYx(200); // Delay of .166ms     //value between 0 255
    	Delay10TCYx(200); // Delay of .166ms
    	Delay10TCYx(200); // Delay of .166ms
    	Delay10TCYx(200); // Delay of .166ms
    	Delay10TCYx(200); // Delay of .166ms
    	Delay10TCYx(200); // Delay of .166ms
   		j++;
 	}   
	return;    
}

#define RESET  PORTDbits.RD3

void main(void)
{
	char add_A = 'A';
	char add_B = 'B';
	char channel = 'C';
	int i=0;
	OpenLCD();
	init_LCD4();
	initialize_system();
	StartUSART();	
	Delay_ms(100);
	RESET = 1;	
	for(i = 0;i<5;i++){
	led_toggle();
	Delay_ms(500);	
	}	
	
/***START COMMUNICATION**************************/	
	
/*	TXREG = add_B;
	while(PIR1bits.TXIF);
	TXREG = add_A;
	while(PIR1bits.TXIF);
	TXREG = channel;
	while(PIR1bits.TXIF);	*/
	
/************************************************************/
	putcUSART(add_B);
	while (BusyUSART());
	Delay_ms(1);
	putcUSART(add_A);
	while (BusyUSART());
	putcUSART(channel);
	Delay_ms(1000);
	
	while(1){
	Delay_ms(500);
	led_toggle();
	}	
}
void initialize_system(){
	//LEDs
	TRISDbits.TRISD0 = 0;
	TRISDbits.TRISD1 = 0;
	TRISDbits.TRISD2 = 0;
	TRISDbits.TRISD3 = 0;
}

void led_toggle(){
	LED1 = !LED1;
	LED2 = !LED2;
	LED3 = !LED3;	
}	
/** EOF Demo02.c *************************************************************/
