/******************************************************************************
 * File Name:	Main.c
 * Program:		Project for Fundamentals of Embedded Systems class
 * Author:		Robert Weber
 * Purpose:		Main file. Contains the Main function and other executive
 *				functions. It's the "master" of the program.
 *
 *  Date	  By:		Changes:
 * ---------  --------	-------------------------------------------------------
 * 24 Apr 02  R Weber	Original file.
 * 24 Feb 04  T Lill	modified for Winter 04 session
 * 22 Apr 05  T Lill	removed deprecated functions
 * 03 Nov 06  T Lill	Simplified heartbeat function
 * 15 Dec 06  T Lill	Updated heartbeat function
 * 01 Nov 10  T Lill	Converted program to ATmega2560
 * 22 Sep 12  T Lill	Changed LED port to D
 * 26 Sep 12  T Lill	ported to Atmel Studio 6
 ******************************************************************************/
#include <avr/io.h>
#include "lib.h"
#include "interrpt.h"
#include "heartbeat.h"
#include "serial.h"

/************************* Function Prototypes ******************************/
int main(void);


/*********************** Function Implementations ******************************/

void heartbeat(void)
{
	// see warning about SET_BIT in lib.h
	asm volatile (" sbi	0x09, 0 ");
}

/*****************************************************************************
 * Main function.
 * 
 * Initializes hardware and software. Then enters the endless foreground loop.
 * 
 *****************************************************************************/
int main(void) 
{
    // Disable interrupts
    cli();

	// Set port D as an output and turn off all LEDs
	DDRD = 0xFF;
	PORTD = 0xFF;

    /* Initialize the Timer 0 */
    ISR_InitTimer0();

	// Initialize serial I/O
	SCIInitialize();
	
	GetStackByteCount();

    /* Enable interrupts. Do as last initialization, so interrupts are
     * not initiated until all of initialization is complete. */
   sei();

   for ( ; ; )		/* Foreground loops forever */
   {   // Do slow tasks here

   }   /* end of endless loop */

   return 0;
}
