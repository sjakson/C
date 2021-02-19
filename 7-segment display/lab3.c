// TinFai Hui (Samuel)
// CECS 347
// Lab 3 (Option 1)
// 10/9/2006

#include <reg52.h>						//use reg52.h for pre-defined SFR


//Define Variables
static unsigned char value1 = 0;	   	//the current value represented by the 7 on-board LEDs
static unsigned char value2 = 0;		//the current value on the 7-segment display


//Function Prototypes
void timer2 (void) interrupt 5			//priority order = 5 for timer 2
{
	TR2 = 0;							//turn off timer 2
	TF2 = 0;							//clear timer 2 flag
		  	
	P1 = value1;						//set P1 for LED output the value
   	if(value1 == 255) 					//00-FF
	{ 	
		value2++;  						//value2 = value2 + 1
		if(value2 > 9)					//value on 7-segment is bigger than 9
			value2 = 0;					//reset to 0
		P2 = value2;					//P2 as the communication port	
	}
	value1++;  							//value1 = value1 + 1
	TR2 = 1;						 	//timer 2 runs
}

//Main Program
void main (void)
{	
	T2CON = 0x00;						//setting the timer 2 into auto-reload mode
	ET2 = 1;                      		//enable Timer 2 Interrupts
	
	RCAP2L = 0x00;
	RCAP2H = 0xB8;                    	//Baud Rate = 
										//Oscillator Frequency/[n * [65536 - (B8,00)
	value1 = 0;							//reset value on the on-board LEDs 
	value2 = 0;							//reset value on the 7-segment display
	P2 = value2;						//P2 as the communication port
	TR2 = 1;  							//timer2 runs
	EA = 1;                       		//enable interrupt
	
	while(1) {}		  					//loop forever, so the program never stops
}