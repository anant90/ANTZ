/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>
#include <delays.h>
#include <usart.h>
#include <timers.h>
#include <adc.h>
#include "lcd.h"
#include "fun.h"

/** A U T H O R:
// Anant Jain
// 2008EE10330
// IIT Delhi
// E: anant.ee108@ee.iitd.ac.in
// Written for EEP211 project and Techfest 2010

/** # D E F I N E S ********************************************************/

#define blockedTrue 0
#define blockedFalse 1
#define WHITE 1
#define BLACK 0
#define True 1
#define False 0

#define pulse 40 // turns
#define pulseFwd 18
#define backPulse 300

#define sp 2

/** INPUTS/OUTPUTS - MAC A***********************************************************/

/** L E D S***********************************************************/

#define LED1 PORTDbits.RD0
#define LED2 PORTDbits.RD1

/////////////////SENSORS//////////////////
#define sensor1 PORTAbits.RA0
#define sensor2 PORTAbits.RA1
#define sensor3 PORTAbits.RA2
#define sensor4 PORTAbits.RA3
#define sensor5 PORTAbits.RA4
// sensor 1 is my leftmost

#define TSOP PORTDbits.RD7

/////////////////SWITCHES//////////////////
#define SW2 PORTEbits.RE0
#define SW3 PORTEbits.RE1
#define SW4 PORTEbits.RE2
#define SW5 PORTCbits.RC0

/** V A R I A B L E S ********************************************************/
unsigned char maze[9][9];
unsigned int path[21][3];
int runCount = 0;
int depositCount = 0;
int otherDepositCount = 0;
int depSqX; int depSqY;
int curX; int curY;
int nextX; int nextY;
int pathIndex;
int curHead;
int Reset = False;
int turn = 1;
int thresh[5];
int threshBlock[3];
char dir[4]={'n','w','s','e'};

// For digital Output:
// s1 is leftmost
int s1() { if(sensor1==1) return 1; else return 0; }
int s2() { if(sensor2==1) return 1; else return 0; }
int s3() { if(sensor3==1) return 1; else return 0; }
int s4() { if(sensor4==1) return 1; else return 0; }
int s5() { if(sensor5==1) return 1; else return 0; }


int frontBlocked() { if(TSOP==0) return blockedTrue; else return blockedFalse;}

int comRx() {
	char a;
	while (DataRdyUSART()==1)
		a=ReadUSART();

	if(a=='1')
		return 1;
	else
		return 0;
}
void comTx(int a) {
	if(a==1) 
		putcUSART('1');
	else if(a==0)
		putcUSART('0');
	Delay_ms(100);
}



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
    ;
}

#pragma code _LOW_INTERRUPT_VECTOR = 0x000818
void _low_ISR (void)
{
    ;
}
#pragma code
/** D E C L A R A T I O N S **************************************************/
#pragma code

void init() {
//--------------------------------------------------------------------//
// Intialize the path
	int x,y;
	path[0][0] = 2; path[0][1]= 2; path[0][2] = 3;
	path[1][0] = 3; path[1][1]= 2; path[1][2] = 3;
	path[2][0] = 4; path[2][1]= 2; path[2][2] = 3;
	path[3][0] = 5; path[3][1]= 2; path[3][2] = 3;
	path[4][0] = 6; path[4][1]= 2; path[4][2] = 0;
	path[5][0] = 6; path[5][1]= 3; path[5][2] = 0;
	path[6][0] = 6; path[6][1]= 4; path[6][2] = 0;
	path[7][0] = 6; path[7][1]= 5; path[7][2] = 1;
	path[8][0] = 5; path[8][1]= 5; path[8][2] = 1;
	path[9][0] = 4; path[9][1]= 5; path[9][2] = 2;
	path[10][0] = 3; path[10][1]= 5; path[10][2] = 2;
	path[11][0] = 3; path[11][1]= 4; path[11][2] = 3;
	path[12][0] = 4; path[12][1]= 4; path[12][2] = 3;
	path[13][0] = 5; path[13][1]= 4; path[13][2] = 2;
	path[14][0] = 5; path[14][1]= 3; path[14][2] = 1;
	path[15][0] = 4; path[15][1]= 3; path[15][2] = 1;
	path[16][0] = 3; path[16][1]= 3; path[16][2] = 1;
	path[17][0] = 2; path[17][1]= 3; path[17][2] = 0;
	path[18][0] = 2; path[18][1]= 4; path[18][2] = 0;
	path[19][0] = 2; path[19][1]= 5; path[19][2] = 0;
	path[20][0] = 2; path[20][1]= 6; path[20][2] = 3;
	path[21][0] = 3; path[21][1]= 6; path[21][2] = 3;
	path[22][0] = 4; path[22][1]= 6; path[22][2] = 3;
	path[23][0] = 5; path[23][1]= 6; path[23][2] = 3;

		
	// Initialize the maze 
	for(x=0;x<9;x++)
		for(y=0;y<9;y++)
			maze[x][y] = 0;
	
	for(x=1;x<8;x++)
		for(y=1;y<8;y++)
			maze[x][y] = 1;
	maze[1][1] = 0;
	maze[7][7] = 0;
	maze[7][1] = 0;
	maze[1][7] = 0;
	maze[2][2] = 0;
	maze[6][6] = 0;
	// Maze initialization complete
	
	runCount +=1;
	depositCount = 0;
	otherDepositCount = 0;
	
	curX = 2;
	curY = 2;
	nextX = 2;
	nextY = 2;
	
	curHead = 3;
	pathIndex = 0;
	comTx(0);

	Reset = False;
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


//-------------------------------------------------------------------//
// Motor Controls:


void openGripper() {
	int z,i,k;
	PORTDbits.RD2 = 1;
	PORTDbits.RD3 = 0;
	Delay_ms(600);
	PORTDbits.RD2 = 0;
	PORTDbits.RD3 = 0;
}
void closeGripper() {
	int z,i,k;

	PORTDbits.RD2 = 0;
	PORTDbits.RD3 = 1;
	Delay_ms(600);	
	PORTDbits.RD2 = 0;
	PORTDbits.RD3 = 0;
}
void loosenGripper() {

	PORTDbits.RD2 = 0;
	PORTDbits.RD3 = 0;
}
void speed(int j, int dir) {
	int i,k;
	if(dir==1) {
	// fwd
	PORTBbits.RB5 = 1;
	PORTBbits.RB7 = 0;
	PORTBbits.RB4 = 1;
	PORTBbits.RB6 = 0;
	}
	else if(dir==2) {
	// left
	PORTBbits.RB5 = 0;
	PORTBbits.RB7 = 1;
	PORTBbits.RB4 = 1;
	PORTBbits.RB6 = 0;
	}
	else if(dir==21) {
	// leftND
	PORTBbits.RB5 = 0;
	PORTBbits.RB7 = 0;
	PORTBbits.RB4 = 1;
	PORTBbits.RB6 = 0;
	}
	else if(dir==3) {
	// right
	PORTBbits.RB5 = 1;
	PORTBbits.RB7 = 0;
	PORTBbits.RB4 = 0;
	PORTBbits.RB6 = 1;
	}
	else if(dir==31) {
	// rightND
	PORTBbits.RB5 = 1;
	PORTBbits.RB7 = 0;
	PORTBbits.RB4 = 0;
	PORTBbits.RB6 = 0;
	}
	else if(dir==4) {
	// back
	PORTBbits.RB5 = 0;
	PORTBbits.RB7 = 1;
	PORTBbits.RB4 = 0;
	PORTBbits.RB6 = 1;
	}
	else if (dir==5) {
	// stop
	PORTBbits.RB5 = 0;
	PORTBbits.RB7 = 0;
	PORTBbits.RB4 = 0;
	PORTBbits.RB6 = 0;
	}
	if(j>5) j=5;
		
	for(i=0;i!=j;i++) {
		Delay_ms(5);	}

	if(dir==3 || dir==31)
		Delay_ms(2);
		
	PORTBbits.RB5 = 0;
	PORTBbits.RB7 = 0;
	PORTBbits.RB4 = 0;
	PORTBbits.RB6 = 0;

	for(k=0;k!=5-j;k++) {
			Delay_ms(5);
		}
}

void fwd() {
	speed(sp,1);
}

void left() {
	speed(sp,2);
}
void leftNonDiff() {
	speed(sp,21);
}
void right() {
	speed(sp, 3 );
}
void rightNonDiff() {
	speed(sp,31);
}
void leftNonDiffBack() {
	speed(sp,41);
}
void rightNonDiffBack() {
	speed(sp,42);
}
void back() {
	speed(sp, 4);
}

void stop() {
	speed(sp,5);
}

void mvfwdpulse() {
	
	int x;
	for(x=0;x<pulseFwd;x++)
	speed(sp,1);
	/*
	int x;

	PORTBbits.RB5 = 1;
	PORTBbits.RB7 = 0;
	PORTBbits.RB4 = 1;
	PORTBbits.RB6 = 0;

	for(x=0;x!=pulse;x++) {
		Delay_ms(1);
	}
	stop();
	*/
}
void rightpulse() {
	int x;
	for(x=0;x<pulse;x++)
	speed(sp,3);
	/*
	int x;

	PORTBbits.RB5 = 1;
	PORTBbits.RB7 = 0;
	PORTBbits.RB4 = 0;
	PORTBbits.RB6 = 1;

	for(x=0;x!=pulse;x++) {
		Delay_ms(1);
	}
	stop();
	*/
}
void leftpulse() {
	int x;
	for(x=0;x<pulse;x++)
	speed(sp,2);

	/*
	int x;

	PORTBbits.RB5 = 0;
	PORTBbits.RB7 = 1;
	PORTBbits.RB4 = 1;
	PORTBbits.RB6 = 0;

	for(x=0;x!=pulse;x++) {
		Delay_ms(1);
	}
	stop();
	*/
}
void mvbwdpulse() {

	int x;
	for(x=0;x<backPulse;x++)
	speed(sp,1);
	
	/*
	int x;

	PORTBbits.RB5 = 0;
	PORTBbits.RB7 = 1;
	PORTBbits.RB4 = 0;
	PORTBbits.RB6 = 1;

	for(x=0;x!=pulse;x++) {
		Delay_ms(1);
	}
	stop();
	*/
} 


//--------------------------------------------------------------------//
void rightTurn() {

}
void leftTurn() {

}
void turnRight() {
	/*
	int i;
	while(s1() != BLACK)
		fwd();
	Delay_ms(1000);
	rightpulse();
	curHead+=3;
	curHead%=4;
	*/


	// Take a right turn !!
	int t=0,s=0;
	while(s1()==WHITE | s5()==WHITE)
		fwd();

	
	while((t<2 | s==0) | s2()==BLACK) {
		rightNonDiff();
		if(s1()==WHITE && t==0) t=1;
		if(s1()==BLACK && t==1) t=2;
		if(s5()==WHITE) s=1;
	}
	fwd();
	curHead+=3;
	curHead%=4;
	// Right turn complete
	
}

void turnLeft() {
	/*
	int i;
	mvfwdpulse();
	Delay_ms(1000);
	leftpulse();
	curHead+=1;
	curHead%=4;
	*/
	
	// Take a left turn !!
	int t=0, s=0;
	while(s1()==WHITE | s5()==WHITE)
		fwd();

	while(t<2 |s==0 | s4()==BLACK) {
		leftNonDiff();
		if(s5()==WHITE && t==0) t=1;
		if(s5()==BLACK && t==1) t=2;
		if(s1()==WHITE) s=1;
	}
	fwd();
	curHead+=1;
	curHead%=4;
	// Left turn complete
	
}

void aboutTurn() {
	
	// Take an about turn from left!!
	int t=0;
	int s=0;

	mvfwdpulse();
	while(t==0 |s==0 | s3()==BLACK) {
		left();
		if(s5()==WHITE) t=1;
		if(s1()==WHITE) s=1;
	}
	t=0; s=0;
		while(t==0 |s==0 | s3()==BLACK) {
		left();
		if(s5()==WHITE) t=1;
		if(s1()==WHITE) s=1;
	}

	fwd();
	curHead+=2;
	curHead%=4;
	// About turn complete

}



void moveStraight() {
	// Makes the bot follow the line till it finds the next intersection and then returns
	while(s1()==WHITE | s5()==WHITE)
		fwd();
//	while(s2()==BLACK)
//		rightNon	
	while(!((s1()==WHITE) & (s5()==WHITE))) {
		fwd();
		if(s2()==BLACK || s4()==BLACK) {
			if (s4()==WHITE) { 
				while(s2()!=WHITE) {
					leftNonDiff();
					LED2=1;
				}
				LED2=0;	
			}
			else {
				while(s4()!=WHITE) {
					rightNonDiff();
					LED1 =1;
				}
				LED1=0;
			}	
		}
	}
}
//	Delay_ms(2000);

void moveBack() {
	// Makes the bot follow the line till it finds the next intersection and then returns
	while(s1()==WHITE)
		back();
	while(!((s1()==WHITE) & (s5()==WHITE))) {
		back();
		if(s2()==BLACK || s4()==BLACK) {
			if(s2()==WHITE) {
				while(s4()!=WHITE)
					rightNonDiffBack();
			}
			else {//s4()==WHITE 
				while(s2()!=WHITE)
					leftNonDiffBack();
			}
		}
	}
}

//--------------------------------------------------------------------//
void move(int x1, int y1, int x2, int y2) {
	int x;
	int distl= 20, distf = 20, distb = 20, distr = 20;
	int lX, lY, fX, fY, rX, rY, bX, bY;

if((x1!=x2) | (y1!=y2)) {

	// Starts at intersection x1,y1 and return when u reach x2,y2
	// moves through the cleared path
	
	/*
	if(comRx()==1) LED1=1; else LED1=0;
	if((x2<2 | x2>6) | (y2<2 | y2 > 2)& !((x1<2 | x1>6) | (y1<2 | y1>2))) { // going from central to one bot zone.. 
		while(comRx()==1);
		while(!turn);
		comTx(1);
	}

	if((x1<2 | x1>6) | (y1<2 | y1 > 2)& !((x2<2 | x2>6) | (y2<2 | y2>2))) { // going from one bot zone to central zone.. 
		comTx(0);
	}
	
	*/

	lX = leftIntX(curX, curHead);
	lY = leftIntY(curY, curHead);
	fX = frontIntX(curX, curHead);
	fY = frontIntY(curY, curHead);
	bX = backIntX(curX, curHead);
	bY = backIntY(curY, curHead);
	rX = rightIntX(curX, curHead);
	rY = rightIntY(curY, curHead);
	
	if(maze[lX][lY]==0)
		distl = dist(lX,lY, x2, y2);
	if(maze[rX][rY]==0)
		distr = dist(rX,rY, x2, y2);
	if(maze[fX][fY]==0)
		distf = dist(fX,fY, x2, y2);
	if(maze[bX][bY]==0)
		distb = dist(bX,bY, x2, y2);
	
	x=20;
	if(distl<x) x = distl;
	if(distf<x) x = distf;
	if(distr<x) x = distr;
	if(distb<x) x = distb;

	
	if(x==distf) {
		moveStraight();
		curX = fX;
		curY = fY;
		move(curX, curY, x2,y2);
	}
	else if(x==distl) {
		leftTurn();	
		moveStraight();	
		curX = lX;
		curY = lY;
		move(curX, curY, x2,y2);
	}
	else if(x==distr) {
		rightTurn();
		moveStraight();		
		curX = rX;
		curY = rY;
		move(curX, curY, x2,y2);
	}
	else { // U turn
		aboutTurn();
		moveStraight();		
		curX = bX;
		curY = bY;
		move(curX, curY, x2,y2);
	}
}
}

void moveOrient (int x1, int y1, int x2, int y2, int dir) {
	int x;
	int distl= 20, distf = 20, distb = 20, distr = 20;
	int lX, lY, fX, fY, rX, rY, bX, bY;
	
	if(comRx()==1) LED1 = 1; else LED1 = 0;
	if((x1==x2) && (y1==y2)) {
		while(curHead != dir) {
			mvbwdpulse();
			leftTurn();
		}
	}
	else {
	// same as move code
	
	if((x2<2 | x2>6) | (y2<2 | y2 > 2)& !((x1<2 | x1>6) | (y1<2 | y1>2))) { // going from central to one bot zone.. 
		while(comRx()==1);
		while(!turn);
		comTx(1);
	}

	if((x1<2 | x1>6) | (y1<2 | y1 > 2)& !((x2<2 | x2>6) | (y2<2 | y2>2))) { // going from one bot zone to one central zone.. 
		comTx(0);
	}
	
	lX = leftIntX(curX, curHead); // 3
	lY = leftIntY(curY, curHead); // 2
	fX = frontIntX(curX, curHead); // 2
	fY = frontIntY(curY, curHead); // 1
	bX = backIntX(curX, curHead); // 2
	bY = backIntY(curY, curHead); // 3
	rX = rightIntX(curX, curHead); // 1
	rY = rightIntY(curY, curHead); // 2
	
	if(maze[lX][lY]==0)
		distl = dist(lX,lY, x2, y2);
	if(maze[rX][rY]==0)
		distr = dist(rX,rY, x2, y2);
	if(maze[fX][fY]==0)
		distf = dist(fX,fY, x2, y2);
	if(maze[bX][bY]==0)
		distb = dist(bX,bY, x2, y2);
	
	x=20;
	if(distl<x) x = distl;
	if(distf<x) x = distf;
	if(distr<x) x = distr;
	if(distb<x) x = distb;
	
	if(x==distf) {
		moveStraight();
		curX = fX;
		curY = fY;
		move(curX, curY, x2,y2);
	}
	else if(x==distl) {
		leftTurn();	
		moveStraight();	
		curX = lX;
		curY = lY;
		move(curX, curY, x2,y2);
	}
	else if(x==distr) {
		rightTurn();
		moveStraight();		
		curX = rX;
		curY = rY;
		move(curX, curY, x2,y2);
	}
	else { // U turn
		aboutTurn();
		moveStraight();		
		curX = bX;
		curY = bY;
		move(curX, curY, x2,y2);
	}
	}
}

void deposit(int x1, int y1, int x2, int y2) { 
	move(x1,y1,x2,y2);
	mvbwdpulse();
	openGripper();
	nextX = backIntX(curX, curHead);
	nextY = backIntY(curY, curHead);
	while(!(s1()==WHITE || s5()==WHITE)) {
		back();
	}
	curX = nextX;
	curY = nextY;
	move(curX, curY, path[pathIndex][0], path[pathIndex][1]);
}

//--------------------------------------------------------------------//
void blockFound() {

	int x1, x2, y1,y2;
	//  The foll. 3 statements are redundant now..
	curX = nextX;
	curY = nextY;
	maze[curX][curY] = 0; // Block cleared.
	
	closeGripper();
	
	if(depositCount==0) {
		
		// Choose a square to deposit
		x1 = abs(4-curX);
		x2 = abs(8-curX);
		y1 = abs(0-curX);
		y2 = abs(4-curY);
		
		if((x1+y1)<=(y2+x2)) {
			depSqX = 4;
			depSqY = 0;
		}
		else {
			depSqX = 8;
			depSqY = 4;
		}
		
	}
	else { // DepositCount = 1 
		if(depSqX == 4) {
			depSqX = 8;
			depSqY = 4;
		}
		else {
			depSqX = 4;
			depSqY = 0;
		}
	}
	
	// Deposition square chosen.
	
	deposit(curX, curY, depSqX, depSqY); // move to destination and come one square back
	moveOrient(curX, curY, path[pathIndex][0], path[pathIndex][1],path[pathIndex][2]);

	// if(no-block-in front)
	depositCount += 1;
	if(depositCount ==2) {
		Reset = True;	
		moveOrient(curX, curY, 2,2,2);
	}
}

void intersection() {
	// Intersection
		curX = nextX;
		curY = nextY;
		maze[curX][curY]=0; // the intersection is cleared
		if(comRx()==1) LED1 =1 ; else LED1 = 0;
			
		if(frontBlocked()==blockedTrue) {
			blockFound();
		}
		/*
		else if(rightBlocked()==blockedTrue) {
			nextX = rightIntX(curX, curHead);
			nextY = rightIntY(curY, curHead); 
				
			move(curX, curY, nextX, nextY);
		}
		else if(leftBlocked()==blockedTrue) {
			nextX = leftIntX(curX, curHead);
			nextY = leftIntY(curY, curHead); 
				
			move(curX, curY, nextX, nextY);
		}
		*/
		else {
			// decide the next intersection on the path
			if(curX==2 & (curY ==2 & curHead==2)) {
				leftTurn();
				intersection();
			}
			else {
				pathIndex+=1;
				nextX = path[pathIndex][0];
				nextY = path[pathIndex][1];
				move(curX, curY, nextX, nextY);
			}
		}
}


void main(void)
{
	int i=0;
	int j = 0;
	////////////////all digital pins////////////////////
	ADCON1 = 0x0F;
	
	//////////motordriver pins/////////////////////
	TRISDbits.TRISD2 = 0;
	TRISDbits.TRISD3 = 0;
	TRISBbits.TRISB4 = 0;
	TRISBbits.TRISB5 = 0;
	TRISBbits.TRISB6 = 0;
	TRISBbits.TRISB7 = 0;
	
	stop();
	
	////////////SENSORS///////////////////
	TRISAbits.TRISA0 = 1;
	TRISAbits.TRISA1 = 1;
	TRISAbits.TRISA2 = 1;
	TRISAbits.TRISA3 = 1;
	TRISAbits.TRISA4 = 1;
	TRISAbits.TRISA5 = 1;
	
	//////////TSOP SENSOR/////////////
	TRISDbits.TRISD7 = 1;
	
	/////////LEDs/////////////////////////
	TRISDbits.TRISD2 = 0;
	TRISDbits.TRISD1 = 0;
	TRISDbits.TRISD0 = 0;
	
	/////////SWITCHES//////////////
	TRISCbits.TRISC0 = 1;
	TRISEbits.TRISE0 = 1;
	TRISEbits.TRISE1 = 1;
	TRISEbits.TRISE2 = 1;

	
	

	OpenLCD();
	init_LCD4();
	
	Delay_ms(100);
	ClearScreen();	

	OpenUSART( USART_TX_INT_OFF &		//Transmit interrupt disabled
				USART_RX_INT_OFF &		//Receiver interrupt disabled
				USART_ASYNCH_MODE &		//Asynchronous mode
				USART_EIGHT_BIT &		//Eight bit data
				USART_CONT_RX &			//Continuous reception
				USART_BRGH_LOW,			//Low baud rate selection
				32 );					//48Mhz/(64*(77+1)) = 9600 baud rate -- 77

	
	while(1){


	 
	// Ready to, wait for GO signal..
	 
	// For machine A: Go is 
	
	if(runCount == 0) {
		// Machine A procedure.
		while(SW2==1) { // Sw1 
			LED2 = 1;
			Delay_ms(20);
			LED2 = 0;
			Delay_ms(20);
		}
	}

	init(); //Initialize the maze, path and other vars.
	
	comTx(1); // Start machine B;
	Delay_ms(100);
	comTx(0);
	
	loosenGripper();

	while(1);
	// To account for the fact that in the start, we are not actually on the intersection.. 
	while(!(s1()==WHITE & s5()==WHITE)) {
		back();
	}
	intersection();
	
	while(Reset == False) {
		while(1){

	
	}
	
	if((s1() == WHITE) & (s5() == WHITE)) { // Intersection
			intersection();
		}
	}
}

}