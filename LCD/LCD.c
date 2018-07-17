/********************************************
; Name: Scott Jackson
; Date: April 8, 2008
; File: LCD.c

; This program displays allows you to type 
; characters on the keyboard of your PC  
; and then have them displayed on your LCD.
; Pushing control and C at the same time  
; will clear the LCD screen.
; Pushing control and A at the same time 
; will set the cursor to line one.
; Pushing control and Z at the same time 
; will set the cursor to line two.
;
; Data is connected to P1
; RS is connected to P2.0 
; RW is connected to P2.1 
; EN is connected to P2.2 
;********************************************/

#include <reg51.h>

 void waitLCD(void);
 void initLCD(void);
 void clearLCD(void);
 void serialSetup(void);
 void waitToRecieve(void);
 void checkControlC(unsigned char value);
 void checkControlA(unsigned char value);
 void checkControlZ(unsigned char value);
 void writeText(unsigned char value);

 sbit EN   = P2^2;
 sbit RW   = P2^1;
 sbit RS   = P2^0;
 sfr ldata = 0x90;	// P1=LCD data pins
 
 void main(void)
 {
 	waitLCD();
	initLCD();
	clearLCD();
	serialSetup();
	waitToRecieve();
 }
  
 void waitLCD()
 {
 	unsigned char value, mask, D7;
	mask = 0x80;
 	EN = 1;					// Start LCD command
	RS = 0;					// Its a command
	RW = 1;				   	// Its a read command
	ldata = 0xFF;			// Set to input
	value = ldata;			// Get data
	D7 = value & mask;
	if(D7 == 0x80)			// Waits until busy flag is low
		waitLCD();
	EN = 0;
	RW = 0;
	return;	
 }

// Initializes LCD 
 void initLCD()
 {
 	EN = 1;
	RS = 0;
	ldata = 0x38;
	EN = 0;
	waitLCD();
	EN = 1;
	RS = 0;
	ldata = 0x0E;
	EN = 0;
	waitLCD();
	EN = 1;
	RS = 0;
	ldata = 0x06;
	EN = 0;
	waitLCD();
	return;
 } 

// Clears the LCD
 void clearLCD()
 {
 	EN = 1;
	RS = 0;
	ldata = 0x01;
	EN = 0;
	waitLCD();
	return;
 }

// Initializes the timing
// and serial
 void serialSetup()
 {
 	TMOD = 0x20;
	TL1 = 0;
	TH1 = -3;
	SCON = 0x50;
	TR1 = 1;
	return;	
 }

// Waits for RI to become high 
// then movs the serial value
// into value.
 void waitToRecieve()
 {
 	unsigned char value;
 	RI = 0;
	while(!RI){}
	value = SBUF;
	RI = 0;
	checkControlC(value);
	waitToRecieve();
 }

// Checks if control C was 
// pushed. If yes clears LCD
 void checkControlC(unsigned char value)
 {
 	if(value != 0x03)
		checkControlA(value);
	clearLCD();
	return;	
 }

// Checks if control A was
// pushed. If yes sets cursor
// to write line one
 void checkControlA(unsigned char value)
 {
 	if(value != 0x01)
		checkControlZ(value);
	EN = 1;
	RS = 0;
	ldata = 0x02;
	EN = 0;
	waitLCD();
	return;
 }

// Checks if control Z was
// pushed. If yes sets cursor
// to write line two 
 void checkControlZ(unsigned char value)
 {
 	if(value != 0x1A)
		writeText(value);
	EN = 1;
	RS = 0;
	ldata = 0xC0;
	EN = 0;
	waitLCD();
	return;	
 }

// Writes to the LCD
 void writeText(unsigned char value)
 {
 	EN = 1;
	RS = 1;
	ldata = value;
	EN = 0;
	waitLCD();
	return;
 }