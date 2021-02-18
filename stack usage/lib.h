/******************************************************************************
 * File Name:	Lib.h
 * Program:		Project for Fundamentals of Embedded Systems class
 * Author:		Robert Weber
 * Purpose:		Header file for lib.c file.
 *
 *  Date	 Changed by: Changes:
 * -------	 ----------- -------------------------------------------------------
 * 18 Feb 04 T Lill		modified for Winter 04 session.  Added 8 MHz clock option.
 * 09 May 05 T Lill		Removed BYTE and WORD definitions.  Added 8.5MHz clock
 *						option.  Added macros to replace deprecated cbi, sbi 
 *						functions. 
 * 03 Nov 06 T Lill		Removed superfluous frequency definitions.
 * 28 Nov 06 T Lill		Updated _itoa and _atoi to allow negative numbers
 * 15 Dec 06 T Lill		Added warning about SET_BIT and optimization.
 * 18 May 08 T Lill		Deleted OSC_FREQ symbol.
 * 26 Sep 12 T Lill		Added F_CPU symbol.
 ******************************************************************************/
#if !defined(LIB_H)		/* Prevents including this file multiple times */
#define LIB_H

#include <avr/io.h>
/*  Define clock speed */

#define F_CPU	8000000UL

#define STRLEN 20			/* Maximum string length for conversions      */

/* Define memory mapping for commonly used registers */

// Define Port B uses
#define HEARTBEAT_LED_BIT				0x00
#define ERROR_LED_BIT					0x01

/* Memory types */
typedef unsigned char *MEMPTR;

typedef enum {
    FALSE = 0,
    TRUE = 1
} eBooleanType;

/* Macros replacing deprecated functions. */
#define CLEAR_BIT(reg, bit) reg = (reg & (~(1 << bit)))
/*
 * WARNING:  SET_BIT may not work properly it the register
 *		is an I/O PINx register.  It works in this case if
 *		optimization is set to -Os, but it fails if the
 *		optimization is set to -O0.  It has not been tested
 *		for other optimization levels.
 */
#define SET_BIT(reg, bit) reg = (reg | (1 << bit))

/* Function Prototypes */
void _itoa(char **,			// Buffer to store converted string into
			int,			// Value to convert
			int);			// Radix (only 10 or 16 allowed)
			
int _atoi(char *, 			// String to convert
			int);			// Radix (only 10 or 16 allowed)

#endif /* LIB_H */
