// ANTZ MAC B
#include<avr/io.h>
#include"fun.h"

#define blockedTrue 0
#define blockedFalse 1
#define WHITE 1
#define BLACK 0
#define True 1
#define False 0

#define led2off PORTB&=0xBF;
#define led2on PORTB|=0x40;

#define led1on PORTC|=0x02;
#define led1off PORTC&=0xFD;

#define sw1 

#define pulse 25
#define sp 4

//---------------------------------------------------------------------
// Global variables:

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

/* For digital Output:
int s1() { if(PINA&0x01) return 1; else return 0; }
int s2() { if(PINA&0x02) return 1; else return 0; }
int s3() { if(PINA&0x04) return 1; else return 0; }
int s4() { if(PINA&0x08) return 1; else return 0; }
int s5() { if(PINA&0x10) return 1; else return 0; }
*/

// For analog output:
int s1() { if(sense(1)<thresh[0]) return WHITE; else return BLACK; }
int s2() { if(sense(2)<thresh[1]) return WHITE; else return BLACK; }
int s3() { if(sense(3)<thresh[2]) return WHITE; else return BLACK; }
int s4() { if(sense(4)<thresh[3]) return WHITE; else return BLACK; }
int s5() { if(sense(5)<thresh[4]) return WHITE; else return BLACK; }



int leftBlocked() { if(senseBlock(1)<threshBlock[0]) return blockedTrue; else return blockedFalse;}
int frontBlocked() { if(senseBlock(2)<threshBlock[1]) return blockedTrue; else return blockedFalse;}
int rightBlocked() { if(senseBlock(3)<threshBlock[2]) return blockedTrue; else return blockedFalse;}

int comRx() {
	return (PINC & 0x20);
}
void comTx(int a) {
	if(a==1) 
		PORTC |= 0x10;
	else if(a==0)
		PORTC &= 0xDF;
}

void init() {
//--------------------------------------------------------------------//
// Intialize the path
	int x,y;
	path[0][0] = 2; path[0][1]= 2; path[0][2] = 2;
	path[1][0] = 3; path[1][1]= 2; path[1][2] = 3;
	path[2][0] = 4; path[2][1]= 2; path[2][2] = 3;
	path[3][0] = 5; path[3][1]= 2; path[3][2] = 3;
	path[4][0] = 6; path[4][1]= 2; path[4][2] = 0;
	path[5][0] = 6; path[5][1]= 3; path[5][2] = 0;
	path[6][0] = 6; path[6][1]= 4; path[6][2] = 0;
	path[7][0] = 6; path[7][1]= 5; path[7][2] = 1;
	path[8][0] = 5; path[8][1]= 5; path[8][2] = 1;
	path[9][0] = 4; path[9][1]= 5; path[9][2] = 2;
	path[10][0] = 4; path[10][1]= 4; path[10][2] = 1;
	path[11][0] = 4; path[11][1]= 3; path[11][2] = 1;
	path[12][0] = 3; path[12][1]= 3; path[12][2] = 1;
	path[13][0] = 2; path[13][1]= 3; path[13][2] = 0;
	path[14][0] = 2; path[14][1]= 4; path[14][2] = 0;
	path[15][0] = 2; path[15][1]= 5; path[15][2] = 0;
	path[16][0] = 2; path[16][1]= 6; path[16][2] = 3;
	path[17][0] = 3; path[17][1]= 6; path[17][2] = 3;
	path[18][0] = 4; path[18][1]= 6; path[18][2] = 3;
	path[19][0] = 5; path[19][1]= 6; path[19][2] = 3;
	path[20][0] = 6; path[20][1]= 6; path[20][2] = 3;
	
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
	
	curHead = 2;
	pathIndex = 0;
	comTx(0);

	Reset = False;
}

void delay_ms(int x) {
	for(int i=0;i!=x;i++) {
			while(!(TIFR & (1<<TOV0)));
			TCNT0 = 0x63;
			TIFR |= (1<<TOV0); /* Timer restarts as soon as you configure this. 
			Hence TCNT0 must be written before TIFR */	
		}

}

void ledToggle() {
	led2on
	delay_ms(50);
	led2off
	delay_ms(50);
}




//-------------------------------------------------------------------//
// Motor Controls:

void openGripper() {
	for(int z=0;z<12;z++) {
		PORTD = 0x20;
		for(int i=0;i!=1;i++) {
			while(!(TIFR & (1<<TOV0)));
			TCNT0 = 0x63;
			TIFR |= (1<<TOV0); /* Timer restarts as soon as you configure this. 
			Hence TCNT0 must be written before TIFR */
		}
		PORTD = 0x00;
		for(int k=0;k!=4;k++) {
			while(!(TIFR & (1<<TOV0)));
			TCNT0 = 0x63;
			TIFR |= (1<<TOV0); 
		}
	}
	PORTD = 0x30;
}
void closeGripper() {
	for(int z=0;z<12;z++) {
		PORTD = 0x10;
		for(int i=0;i!=1;i++) {
			while(!(TIFR & (1<<TOV0)));
			TCNT0 = 0x63;
			TIFR |= (1<<TOV0); /* Timer restarts as soon as you configure this. 
			Hence TCNT0 must be written before TIFR */
		}
		PORTD = 0x00;
		for(int k=0;k!=4;k++) {
			while(!(TIFR & (1<<TOV0)));
			TCNT0 = 0x63;
			TIFR |= (1<<TOV0); 
		}
	}
	PORTD = 0x10;
}

void speed(int j, int dir) {
	if(j>5) j=5;
		PORTB = dir;
		for(int i=0;i!=j;i++) {
			while(!(TIFR & (1<<TOV0)));
			TCNT0 = 0x63;
			TIFR |= (1<<TOV0); /* Timer restarts as soon as you configure this. 
			Hence TCNT0 must be written before TIFR */
		}
		PORTB = 0x00;
		for(int k=0;k!=5-j;k++) {
			while(!(TIFR & (1<<TOV0)));
			TCNT0 = 0x63;
			TIFR |= (1<<TOV0); 
		}

}


void fwd() {
	speed(sp,0x0A);
}

void left() {
	speed(sp,0x09);
}
void leftNonDiff() {
	speed(sp,0x08);
}
void right() {
	speed(sp, 0x06 );
}
void rightNonDiff() {
	speed(sp,0x02);
}
void back() {
	speed(sp, 0x05);
}

void stop() {
	PORTB = 0x00;
}

void mvfwdpulse() {
	int x;
	PORTB = 0x0A;
	for(x=0;x!=pulse;x++) {
		delay_ms(1);
	}
	stop();
}
void mvbwdpulse() {
	int x;
	PORTB = 0x05;
	for(x=0;x!=pulse;x++) {
		delay_ms(1);
	}
	stop();
} 


//--------------------------------------------------------------------//
void rightTurn() {
	// Take a right turn !!
	int t=0,s=0;
	mvfwdpulse();
	
	while((t==0 | s==0) | s3()==BLACK) {
		right();
		if(s1()==WHITE) t=1;
		if(s5()==WHITE) s=1;
	}
	fwd();
	curHead+=3;
	curHead%=4;
	// Right turn complete
}

void leftTurn() {
	// Take a left turn !!
	int t=0, s=0;
	mvfwdpulse();

	while(t==0 |s==0 | s3()==BLACK) {
		left();
		if(s5()==WHITE) t=1;
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
	mvfwdpulse();
	while(!((s1()==WHITE) & (s5()==WHITE))) {
		fwd();
		if(s3()==BLACK) {
			if(s2()==WHITE) {
				while(s3()!=WHITE)
					rightNonDiff();
			}
			else {//s4()==WHITE 
				while(s3()!=WHITE)
					leftNonDiff();
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
	
	if(comRx()==1) led1on else led1off
	if((x2<2 | x2>6) | (y2<2 | y2 > 2)& !((x1<2 | x1>6) | (y1<2 | y1>2))) { // going from central to one bot zone.. 
		while(comRx()==1);
		while(!turn);
		comTx(1);
	}

	if((x1<2 | x1>6) | (y1<2 | y1 > 2)& !((x2<2 | x2>6) | (y2<2 | y2>2))) { // going from one bot zone to central zone.. 
		comTx(0);
	}
	
	
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
	
	if(comRx()==1) led1on else led1off
	if((x1==x2) && (y1==y2)) {
		while(curHead != dir)
			leftTurn();
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
	nextX = backIntX(curX, curHead);
	nextY = backIntY(curY, curHead);
	while(!(s1==WHITE | s5==WHITE)) {
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
		if(comRx()==1) led1on else led1off
			
		if(frontBlocked()==blockedTrue) {
			blockFound();
		}
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
int sense(int index) {
	if(index==1) {
		ADMUX = 0x40;
	}
	if(index==2) {
        ADMUX = 0x41; 
    }
	if(index==3) {
		ADMUX = 0x42; 
    }
	if(index==4) {
		ADMUX = 0x43; 
    }
	if(index==5) {
		ADMUX = 0x44; 
    }
	ADCSRA = 0xE7;
	while(!(ADCSRA & (1<<ADIF)));
	return ADC;
	ADCSRA = 0x00; // Disable the ADC
}
int senseBlock(int index) {
	if(index==1) {
		ADMUX = 0x45;
	}
	if(index==2) {
        ADMUX = 0x46; 
    }
	if(index==3) {
		ADMUX = 0x47; 
    }
	ADCSRA = 0xE7;
	while(!(ADCSRA & (1<<ADIF)));
	return ADC;
	ADCSRA = 0x00; // Disable the ADC
}

void calibrateLine() {
	while((PINB & 0x10)) // Sw1 
		ledToggle();
	
	int s1w = sense(1);
	int s2w = sense(2);
	int s3w = sense(3);
	int s4w = sense(4);
	int s5w = sense(5);
	delay_ms(300);
	
	while((PINB & 0x10)) // Sw1 
		ledToggle();
	
	int s1b = sense(1);
	int s2b = sense(2);
	int s3b = sense(3);
	int s4b = sense(4);
	int s5b = sense(5);
	
	thresh[0] = (s1w+s1b)/2;
	thresh[1] = (s2w+s2b)/2;
	thresh[2] = (s3w+s3b)/2;
	thresh[3] = (s4w+s4b)/2;
	thresh[4] = (s5w+s5b)/2;
	led2off
}

void calibrateBlock() {
	ledToggle();
	
	int l1 = senseBlock(1); // for left
	delay_ms(300);
	ledToggle();
	
	int l2 = senseBlock(1); // for left
	delay_ms(300);
	ledToggle();
	
	int f1 = senseBlock(2); // for front
	delay_ms(300);
	ledToggle();
	
	int f2 = senseBlock(2); // for front
	delay_ms(300);
	ledToggle();
	
	int r1 = senseBlock(2); // for right
	delay_ms(300);
	ledToggle();
	
	int r2 = senseBlock(2); // for right
	
	threshBlock[0] = (l1+l2)/2;
	threshBlock[1] = (f1+f2)/2;
	threshBlock[2] = (r1+r2)/2;
	led2off
}
int main(void) {

	TCNT0 = 0x63;
	TCCR0 = 0x05;

	DDRA = 0x00;
	PORTA = 0xFF;
	
	DDRB = 0b11101111; // 1 for output
	PORTB = 0b00010000; // 1 for input
	
	DDRC = 0x11;
	PORTC = 0xEE;
	
	// Reserve LED D1 for indicating other's one bot zone
	
	DDRD = 0xF0;
	PORTD = 0x0F;

	// Pin config complete
	
	// calibrateLine();
	 calibrateBlock();
	
	openGripper();
	 
	 /*
		while(1) {
		if(leftBlocked()==blockedTrue)
			led2on
		else
			led2off
	}
	*/
	// Ready to, wait for GO signal..
	 
	// For machine B: Go is 
	
	if(runCount == 0) {
		// Machine B procedure.
		while(comRx()!=1) { // Sw1 
		led2on
		delay_ms(20);
		led2off
		delay_ms(20);
	
	}
	}

	init(); //Initialize the maze, path and other vars.
	
	comTx(1); // Start machine B;
	delay_ms(100);
	comTx(0);

	// To account for the fact that in the start, we are not actually on the intersection.. 
	while(!(s1()==WHITE | s5()==WHITE)) {
		back();
	}
	intersection();
	
	while(Reset == False) {
		/*
		fwd();
		if(s3==BLACK) {
			if(s2==WHITE) {
				while(s3!=WHITE)
					rightNonDiff();
			}
			else {//s4==WHITE 
				while(s3!=WHITE)
						leftNonDiff();
			}
		}
		else */if((s1() == WHITE) & (s5() == WHITE)) { // Intersection
			intersection();
		}
	}
	
	return 0;
}
