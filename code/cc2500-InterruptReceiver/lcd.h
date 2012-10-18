#ifndef _LCD
#define _LCD

//#define MDRIVER	PORTDbits.RD3
//#define USART_ABS_SEL PORTBbits.RB2


////////////LCD/////////////////////
//#define BackL	PORTDbits.RD3
#define RS		PORTBbits.RB7
//#define RW		PORTBbits.RB6
#define E		PORTBbits.RB5
//#define DAT0	PORTBbits.RB3
//#define DAT1	PORTBbits.RB2
//#define DAT2	PORTBbits.RB1
//#define DAT3	PORTBbits.RB0
#define DAT4	PORTDbits.RD7
#define DAT5	PORTDbits.RD6
#define DAT6	PORTDbits.RD5
#define DAT7	PORTDbits.RD4


///// sensors/////////
#define outsense	PORTEbits.RE2
#define clksense	PORTCbits.RC1
#define selectsense	PORTCbits.RC2
#define sensor1	PORTCbits.RC3
#define sensor2	PORTAbits.RA5
//#define sensor11 PORTEbits.RE0
//#define sensor12 PORTEbits.RE1


#define banebot	PORTDbits.RD1

///// switches/////////
#define outswitch PORTBbits.RB6
#define clkswitch PORTCbits.RC1
#define selectswitch PORTCbits.RC2
#define PGM		PORTBbits.RB4
///// LEDs/////////
#define outled PORTDbits.RD0
#define clkled PORTDbits.RD1
//////////////////////


extern int CX;
extern int ks;
extern int spd;
extern int TIME,TIME5;

/*			LCD2.c			*/
void init_LCD4(void);
void OpenLCD(void);
void WriteCmd(unsigned char a);
void WriteChar(unsigned char a);
void WriteString(char* tolcd);
void WriteStringL(char* tolcd,int l);
void WriteInt(int i,int l);
void WriteLong(long i,int l);
void WriteULong(unsigned long i,int l);
void ClearScreen(void);
void ResetCursor(void);
void MoveToDisplay(unsigned char a);
void ShiftDisplayRight(int i);
void ShiftDisplayLeft(int i);

/*		   Delay.c			*/
void Delay_s(int i);
void Delay_ms(int i);
void Delay_us(int i);

/*		   init.c			*/
void initialize_system(void);
void StartUSART(void);

/** L E D ***********************************************************/
#define LED1			PORTDbits.RD2
#define LED2             	PORTDbits.RD1
#define LED3              	PORTDbits.RD0



/*			motors.c		*/
void LeftMotor(int a);
void RightMotor(int a);
void StopAllMotors(void);


/*			main.c			*/
void InterruptHandlerHigh(void);
void led_toggle(void);

#endif