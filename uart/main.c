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

int getchar (void);
int putchar (int ch);

int ch;
int i;
int j;


char banner1[] = "**********Welcome to CECS 447**********";
char banner2[] = "*************Scott Jackson*************";
char banner3[] = "************October 13 2009************";
char banner4[] = "**************John Tramel**************";
//October 13 2009
//John Tramel";

char prompt[] = "scott>"; //prompt
char home[] = " signal hill "; //home to print when '*' key pressed
int counter=5; //counter to indicate current cursor position on line

int main(void)
{

PINSEL0 = 0x00000005; // Enable RxD0 and TxD0
U0LCR =   0x00000083; // 8 bits, no Parity, 1 stop bit
U0DLL =   0x000000C2; // 9600 Baud rate @ 30 MHz VPB Clock
U0LCR =   0x00000003; // DLAB = 0



for (i=0; i<sizeof(banner1)-1; i++) //print first line of banner
{
	ch = banner1[i];
	putchar(ch);
}

	putchar(0x0D); //send CR
	putchar(0x0A); //send LF

for (i=0; i<sizeof(banner2)-1; i++) //print second line of banner
{
	ch = banner2[i];
	putchar(ch);
}

	putchar(0x0D); //send CR
	putchar(0x0A); //send LF

for (i=0; i<sizeof(banner3)-1; i++) //print third line of banner
{
	ch = banner3[i];
	putchar(ch);
}

	putchar(0x0D); //send CR
	putchar(0x0A); //send LF

for (i=0; i<sizeof(banner4)-1; i++) //print fourth line of banner
{
	ch = banner4[i];
	putchar(ch);
}

	putchar(0x0D); //send CR
	putchar(0x0A); //send LF

	putchar(0x0D); //send CR
	putchar(0x0A); //send LF





for (i=0; i<sizeof(prompt)-1; i++) //print prompt
{
	ch = prompt[i];
	putchar(ch);
}
 
while(1)
{

	ch = getchar(); //get character from keyboard

	switch (ch) {

 		case 0x0D:	//if CR 'enter or ctrl+m' keys are pressed or LF 'ctrl+j' keys are pressed
		case 0x0A:
			putchar(0x0D); //send CR
			putchar(0x0A); //send LF

			for (i=0; i<sizeof(prompt)-1; i++) //print prompt
			{
				ch = prompt[i];
				putchar(ch);
			}
			counter=5; //reset counter to position 5
		  	break;

		case 0x08: //if 'backspace' key is pressed
			if (counter>5){	//check to make cursor will not delete prompt
				putchar(ch); //send backspace
				putchar(0x20); //send space
				putchar(0x08); //send backspace
				counter-=1;	//decrease counter to indicate move cursor one spot to left
			}
			break;

		case 0x2A: //if '*' key is pressed
			  for (i=0; i<sizeof(home)-1; i++)	//print home city
				{
				ch = home[i];
				putchar(ch);
				}
			counter+=13; //increase counter to indicate move cursor 11 spots to right
		  	break;

 		default:  //if none of above conditions satisfied send default character
  			putchar(ch);
			counter+=1;	//increase counter to indicate move cursor one spot to right
  			break;
 	}




}


}

int getchar (void) //function to get character from the keyboard and send it to the LPC2148
{
	while (!(U0LSR & 0x01));

	return (U0RBR);
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




