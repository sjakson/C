/******************************************************************************
 * File Name:	menu.c
 * Program:		Project for Real-Time Embedded Systems Programming class
 * Author:		Robert Weber
 * Purpose:		Contains menu functions.
 *
 *  Date	Changed by:	Changes:
 * -------	-----------	-------------------------------------------------------
 * 17Aug03	R Weber		Copied from 68HC11 code.
 * 02Feb04  R Weber     Split help menu into two sets of writes to prevent Tx
 *                      buffer overflow without increasing Tx buffer size.
 * 21Mar05  B. Burr     Added stack usage code contributed by Bruce Burr.
 * 20Oct05	T Lill		Editorial changes.  Removed "te" command from help
 *						menu.
 * 05Dec07	T Lill		Changed 'debug' prompt to 'cmd.'
 * 22Sep12	T Lill		Removed unused LCD functions and variables.
 ******************************************************************************/
#include "string.h"
#include "stdlib.h"
#include "lib.h"
#include "serial.h"
#include "errors.h"
#include "menu.h"

#define MAX_MEM_SIZE 0x40
#define MAX_MEM_ADDR 0x21FF
#define MIN_MEM_ADDR 0x200


// Enumeration for the menuing system
typedef enum {
    TOP_MENU,
    DISPLAY_HELP_MENU1,
    DISPLAY_HELP_MENU2,
    DISPLAY_HELP_MENU3,
    DISPLAY_HELP_MENU4,
    DISPLAY_HELP_MENU5,
    DISPLAY_HELP_MENU6,
	MEMORY_GET_ADDRESS,
	MEMORY_GET_LENGTH
} DebugMenuStateType;

typedef enum {
	READ_MEMORY,
    WRITE_MEMORY
} DebugMenuSubType;

/* function prototypes */
int GetStackByteCount(void);

/******************************************************************************
 * Processes keypresses received via RS-232. Implements a menuing system.
 ******************************************************************************/
static char zInputStr[MAX_IN_STR_SIZE];
static char *ptrInputStr = zInputStr;
static DebugMenuStateType MenuState = TOP_MENU;

int DisplaySamples = 0;

void RunMenu(void)
{
    char cTempChar = 1;     // Set to any value other than 0
    char zOutputStr[MAX_MEM_SIZE + 3];  // Add space for newline, return and NULL
    char *ptrOutputStr;
    eErrorType error = NO_ERROR;
    static DebugMenuSubType MenuAction = READ_MEMORY;
	static unsigned int i, Address = 0, Length = 0, Value = 0;
	int StackByteCount;

    // Read input characters until input buffer is empty
    while ((cTempChar = SCIReadChar()) != 0)
    {   // Have another character from input buffer
        if (cTempChar == '\r')
        {   // Enter character. Process input
            *ptrInputStr = '\0';        // append Null
            ptrInputStr = zInputStr;    // Reset input string
            SCIWriteString_P(PSTR("\n\r"));     // Move cursor to next line

			// Reset displaying of A/D samples
            DisplaySamples = 0;

            // Process entry based on debug menu state
            switch(MenuState)
            {
                case TOP_MENU:
                    if (strcmp(zInputStr, "?") == 0)
                    {   // Help screen
						MenuState = DISPLAY_HELP_MENU1;
                    }

                    else if (strcmp(zInputStr, "ge") == 0)
                    {   // Display desired signal parameters
                        // Retrieve signal parameters
                        error = GetError();
                        SCIWriteString_P(PSTR("  Error = "));
                        ptrOutputStr = zOutputStr;
                        _itoa(&ptrOutputStr, error, 10);
                        SCIWriteString(zOutputStr);
                        SCIWriteString_P(PSTR("\n\r"));
                    }

                    else if (strcmp(zInputStr, "ce") == 0)
                    {
                        ClearError();
                    }

					else if ((strcmp(zInputStr, "rm") == 0) ||
                             (strcmp(zInputStr, "wm") == 0))
                    {   // Memory menu
                        if (strcmp(zInputStr, "rm") == 0)
                        {
                            MenuAction = READ_MEMORY;
                        }
                        else
                        {
                            MenuAction = WRITE_MEMORY;
                        }

                        // Get address
                        SCIWriteString("  Starting address in hex (200 to 21FF) = ");
                        MenuState = MEMORY_GET_ADDRESS;
                    }

					else if (strcmp(zInputStr, "sk") == 0) 
					{
						StackByteCount = GetStackByteCount();
                        SCIWriteString_P(PSTR("Stack Byte Count = "));
                        ptrOutputStr = zOutputStr;
                        _itoa(&ptrOutputStr, StackByteCount, 10);
                        SCIWriteString(zOutputStr);
                        SCIWriteString_P(PSTR("\n\r"));
					}

				   else
                    {   // No entry
                        // Back to top menu
                        MenuState = TOP_MENU;
                    }
                    break;

				case MEMORY_GET_ADDRESS:
                    if (zInputStr[0] != '\0')
                    {   // Just skip NULL entries
                        Address = _atoi(zInputStr, 16);
						
						if ((Address < MIN_MEM_ADDR) ||
						    (Address > MAX_MEM_ADDR))
						{	// Address out of range
							SCIWriteString_P(PSTR("  Address out of range\n\r"));
						}
						else
						{
							if (MenuAction == READ_MEMORY)
							{
								// Now get length
								SCIWriteString_P(PSTR("  Length in hex (max 40) = "));
							}
							else
							{
								SCIWriteString_P(PSTR("  Value (in hex) = "));
							}
							MenuState = MEMORY_GET_LENGTH;
						}
					}
					else
					{   // No entry
						// Back to top menu
						MenuState = TOP_MENU;
					}
                    break;

               case MEMORY_GET_LENGTH:
                    if (zInputStr[0] != '\0')
                    {
                        if (MenuAction == READ_MEMORY)
                        {
                            Length = _atoi(zInputStr, 16);
							
							// Limit memory to available RAM
							if ((Address + Length - 1) > MAX_MEM_ADDR)
							{
								Length = MAX_MEM_ADDR - Address + 1;
							}
							
                            if (Length == 0)
                            {   // default to 1
                                Length = 1;
                            }

                            if (Length > MAX_MEM_SIZE)
                            {
                                Length = MAX_MEM_SIZE;
                            }

                            SCIWriteString_P(PSTR("  Memory ="));

                            for (i = 0; i < Length; ++i)
                            {
                                if ((i % 16) == 0)
                                {
                                    SCIWriteString_P(PSTR("\n\r  "));

                                    // Display address
                                    ptrOutputStr = zOutputStr;
                                    _itoa(&ptrOutputStr, Address, 16);
                                    SCIWriteString(zOutputStr);
                                    SCIWriteString(">  ");
                                }

                                ptrOutputStr = zOutputStr;
                                *ptrOutputStr = *(unsigned char *)Address;
                                ++Address;

                                // Convert character to an ASCII string in Hex
                                _itoa(&ptrOutputStr, (int)zOutputStr[0], 16);

                                // Force to be 2 characters long
                                if (strlen(zOutputStr) < 2)
                                {
                                    zOutputStr[1] = zOutputStr[0];
                                    zOutputStr[0]= '0';
                                }
                                zOutputStr[2] = ' ';
                                zOutputStr[3] = '\0';
                                SCIWriteString(zOutputStr);
                            }

                            SCIWriteString_P(PSTR("\n\r"));
                        }
                        else
                        {   // Write memory
                            Value = _atoi(zInputStr, 16);

                            if (Value > 0xFF)
                            {   // invalid value
                                SCIWriteString_P(PSTR("Invalid value\n\r"));
                                MenuState = TOP_MENU;
                            }
                            else
                            {
                                SCIWriteString_P(PSTR("Write Mem\n\r"));
                                ptrOutputStr = (char *)Address;
                                *ptrOutputStr = (char)Value;

                                SCIWriteString_P(PSTR("\n\r"));
                            }
                        }

                        // Back to top menu
                        MenuState = TOP_MENU;
                    }
					
                    break;

				default:
                    // Erroneus state. Reset to none
                    MenuState = TOP_MENU;
                    break;
            }   // end switch

            // Reset string pointer. May have been moved during command processing
            ptrInputStr = zInputStr;

            if (MenuState == TOP_MENU)
            {
                // Display prompt
                SCIWriteString_P(PSTR("cmd> "));
            }
        }
        else
        {   // Save new character to input buffer
            if (ptrInputStr < &zInputStr[MAX_IN_STR_SIZE-2])
            {   // Buffer is not full
                *ptrInputStr = cTempChar;
                *(ptrInputStr+1) = '\0';    // Keep null in string

                // echo character
                SCIWriteString(ptrInputStr);

                ++ptrInputStr;
            }   // else, buffer is full. Ignore characters.
        }
    }   // End while. All characters processed

	if (MenuState == DISPLAY_HELP_MENU1)
	{	// Display 1st part of help menu
		SCIWriteString_P(PSTR("  Commands are:\n\r"));
		SCIWriteString_P(PSTR("  ge  - Display error code\n\r"));
		MenuState = DISPLAY_HELP_MENU2;
	}
	
	else if (MenuState == DISPLAY_HELP_MENU2)
	{	// Display 2nd part of help menu
		SCIWriteString_P(PSTR("  ce  - Clear current error\n\r"));
		MenuState = DISPLAY_HELP_MENU3;
	}
	
	else if (MenuState == DISPLAY_HELP_MENU3)
	{	// Display 3rd part of help menu
		SCIWriteString_P(PSTR("  sk  - stack used\n\r"));		
		MenuState = DISPLAY_HELP_MENU4;
	}

	else if (MenuState == DISPLAY_HELP_MENU4)
	{	// Display 4th part of help menu - not used here
		MenuState = DISPLAY_HELP_MENU5;
	}

	else if (MenuState == DISPLAY_HELP_MENU5)
	{	// Display 5th part of help menu
		SCIWriteString_P(PSTR("  rm  - Read memory\n\r"));
		MenuState = DISPLAY_HELP_MENU6;
	}

	else if (MenuState == DISPLAY_HELP_MENU6)
	{	// Display 6th part of help menu
		SCIWriteString_P(PSTR("  wm  - Write memory\n\r"));
		SCIWriteString_P(PSTR("  ?   - Display this help menu\n\r"));
		MenuState = TOP_MENU;
	}
}

/* The test pattern written in RAM */
#define RAM_PATTERN  		((unsigned char)0xAA)
/* The minimum number of times this pattern must occur consecutively in memory */
#define RAM_MIN_PATTERN		3


/* This utility finds the first three occurrences of the pre-written 
	test pattern 0xAA. This pattern is written in to all RAM
	memory locations before any other initialization takes place.
	The value returned is the maximum size of the stack in bytes.
*/
int GetStackByteCount(void)
{ 
	/* Assignment: Determine how much stack has been used. One method for doing this
	 * would be:
	 * 1. Starting at the end of RAM (0x21FF), check for the pattern 0xAA.
	 * 2. Continue backwards in RAM until either:
	 *    a) You find 3 0xAA patterns. We use multiple patterns just in case
	 *       a value in use happens to be 0xAA.
	 *    b) You get to the start of RAM at 0x200. Clearly, all RAM has been
	 *       used at some point, and somehow, you didn't crash.
	 */
	
	int StackCount = 0; // Counter to keep track of total stack usage
	
	int PatternCount = 0; // Counter to keep track of number of 0xAA's in a row
			
	unsigned char *stkPtr; // Pointer to pointer to current value in RAM
	
	unsigned char prevValue = 0; // used to store previous value
	
	for (stkPtr=(unsigned char*)MAX_MEM_ADDR; stkPtr>=(unsigned char*)MIN_MEM_ADDR; stkPtr--) //Start stkPtr at top of RAM 0x21FF
	{
		if(prevValue == RAM_PATTERN)	//check if previous value of SP is 0xAA
		{
			if (*stkPtr == RAM_PATTERN) //check if current value of SP is 0xAA. If so increase pattern count
				PatternCount++;
			else
				PatternCount=0;		// Reset pattern count back to zero if current value is not 0xAA				
		}
		else  //previous value is not 0xAA 
		{
			if (*stkPtr == RAM_PATTERN) // If current value is 0xAA increase pattern count
			PatternCount++;
		}		
				
		prevValue = *stkPtr; // Set current value to previous value
		StackCount++;	
		
		if (PatternCount == RAM_MIN_PATTERN) //check for 3 0xAA's in a row
			return StackCount-3; // Subtract off 0xAA's from count and return value
	}	
	return StackCount;

}

