//USART COMMUNICATION for asynchronous mode.
#include <p18cxxx.h>
	
	void StartUSART(){
	TXSTAbits.BRGH = 1;		//high baud rate select
	BAUDCONbits.BRG16 = 0;	//8-bit baud rate generator - SPBRG only
	SPBRGH = 0;
	SPBRG = 77;				//baud rate = 38400
	
	TXSTAbits.SYNC = 0;	//ASYNCHRONOUS MODE
	RCSTAbits.SPEN = 1;	//serial port enabled
	TRISCbits.TRISC7 = 1;	//configuring tx and rx pins as usart.
	TRISCbits.TRISC6 = 1;
	
	//TXSTAbits.CSRC = 1;	//dont care in asynchronous mode
	BAUDCONbits.RXDTP = 0;	//received data polarity select bit
	BAUDCONbits.TXCKP = 0;	//clk and data polarity select bit
	RCSTAbits.RX9 = 0;		//8 BIT reception
	TXSTAbits.TX9 = 0;		// 8-bit transmission
	
	TXSTAbits.SENDB = 0;	//?? :( sync Break transmission completed
	//TXSTAbits.TRMT		READ ONLY 
	//TXSTAbits.TX9D		//R/W 9TH bit
	
	RCONbits.IPEN = 1;		//enable interrupt priority feature
	IPR1bits.RCIP = 1;		//interrupt priority high
	PIE1bits.RCIE = 1;		//receive interrupt enable
	INTCONbits.GIEH = 1;	//enable all high priority interrupts
	
	//RCSTAbits.SREN 		//dont care in asynchronous mode
	//RCSTAbits.ADDEN 
	//RCSTAbits.FERR		//READ ONLY no framing error
	//RCSTAbits.OERR		//READ ONLY overrun error
	//RCSTAbits..RX9D 		//READ ONLY 9th bit received.
	//BAUDCONbits.ABDOVF 	used in auto baud acquisition mode
	//BAUDCONbits.RCIDL		is receive operation idle??
	
	TXSTAbits.TXEN = 1;	//transmit enable
	RCSTAbits.CREN = 1;	//enable receiver
	
	/*COMMENTS***********************************
	> If using interrupts enable GIE AND PEIE of INTCON register
	> data to be transmitted should be written in TXREG register
	> received data stored in RCREG
	> if any error occured while clear and reenable CREN
	> reception : PIR1bits.RCIF, PIE1bits.RCIE, IPR1bits.RCIP
	> transmission : PIR1bits.TXIF, PIE1btis.TXIE, IPR1bits.TXIP
	>INTERRUPT FLAGS are read only
	****************************************************/
}	

