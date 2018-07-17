/********************************************************/
/* Program For "Olimex ARM7 USB-LPC2148" */
/* Target MCU : Philips ARM7-LPC2148 */
/* : X-TAL : 12.00 MHz */
/* : Run Speed 60.00 MHz (With PLL) */
/* : PLL Setup = M(3),P(4) */
/* : VPB Clock = (1/2)CPU Clock = 30.00 MHz */
/* Keil Editor : uVision3 V4.0.0.524 */
/* Compiler : Keil CARM V2.50a */
/* Create By : Scott Jackson CSULB */
/* Last Update : 13October2009 */
/* Function : Interface with UART with keyboard and hyperterm */
/********************************************************/
// Want characters typed in keyboard to be processed through UART and sent to the monitor
#include "LPC214x.H" // LPC2148 MPU Registers
//#include <stdio.h>

int getchar (void);
int putchar (int ch);
void delay(unsigned long int count1); 

int ch;
int i;
//int j;
float pot;
float pi = 3.141592;
int newval;
char volt[4];
unsigned val = 0;
float val1, val2;
unsigned val3, val4;

char banner1[] = "**********Welcome to CECS 447**********";
char banner2[] = "*************Scott Jackson*************";
char banner3[] = "************November 5 2009************";
char banner4[] = "**************John Tramel**************";
//October 13 2009
//John Tramel";

char prompt[] = "scott>"; //prompt
//char home[] = " signal hill "; //home to print when '*' key pressed
int counter=5; //counter to indicate current cursor position on line

int main(void)
{

PINSEL0 = 0x00000005; // Enable RxD0 and TxD0
U0LCR =   0x00000083; // 8 bits, no Parity, 1 stop bit
U0DLL =   0x000000C2; // 9600 Baud rate @ 30 MHz VPB Clock
U0LCR =   0x00000003; // DLAB = 0

VPBDIV  = 0x00000002;
IODIR1  = 0x00FF0000;
PINSEL1 = 0x100C0000;
AD0CR   = 0x00210608;
AD0CR  |= 0x01000000;


//for (i=0; i<sizeof(banner1)-1; i++) //print first line of banner
//{
//	ch = banner1[i];
//	putchar(ch);
//}
//
//	putchar(0x0D); //send CR
//	putchar(0x0A); //send LF
//
//for (i=0; i<sizeof(banner2)-1; i++) //print second line of banner
//{
//	ch = banner2[i];
//	putchar(ch);
//}
//
//	putchar(0x0D); //send CR
//	putchar(0x0A); //send LF
//
//for (i=0; i<sizeof(banner3)-1; i++) //print third line of banner
//{
//	ch = banner3[i];
//	putchar(ch);
//}
//
//	putchar(0x0D); //send CR
//	putchar(0x0A); //send LF
//
//for (i=0; i<sizeof(banner4)-1; i++) //print fourth line of banner
//{
//	ch = banner4[i];
//	putchar(ch);
//}
//
//	putchar(0x0D); //send CR
//	putchar(0x0A); //send LF
//
//	putchar(0x0D); //send CR
//	putchar(0x0A); //send LF
//
//
//
//
//
//for (i=0; i<sizeof(prompt)-1; i++) //print prompt
//{
//	ch = prompt[i];
//	putchar(ch);
//}
 

	

	



	while(1)
	{
	
		do
		{
			val = AD0DR3;
		}
	
		while ((val & 0x80000000) == 0);
		val = ((val >> 6) & 0x03FF);//<<12;
	
		

	 	pot = val * 3.3 / 1023;
	
	   	sprintf(volt, "%f", pot);
	
	
		for (i=0; i<sizeof(volt); i++)
		{
			ch = volt[i];
			putchar(ch);
		}

		putchar(0x0D); //send CR

		//val = val << 6;		
	   	//DACR = val;

		for (i=0; i<=360; i++)
		{
			delay(val);
			val1 = (i*pi)/180.0;	
			val2 = (cos(val1)*511)+512;
			val3 = val2;		
			val4 = val3 << 6;		
	   		DACR = val4;
		 }



	}	



}



int putchar (int ch)//function to to send character back to the computer
{
	if (ch == '\n') {
		while (!(U0LSR & 0x20));
		U0THR = 0x0D;  
	}					 
	while (!(U0LSR & 0x20));
	return (U0THR = ch);
}	

void delay(unsigned long int count1) 
{ 
  while(count1 > 0) {count1--;}                        // Loop Decrease Counter    
} 


