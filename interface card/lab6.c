//Tin Fai Hui (Samuel)
//CECS 347
//Lab 6
//11/15/2006


#include <REG52a.h>									//use reg52.h for pre-defined SFR	
#define XTAL 11059200								//crystal oscillator frequency = 11.0592MHz


//Initialize Variables-----------------------------------------------------------------------------------
unsigned char timer = 0; 	  						//a timer variable that count up to 0.5 sec
bit signal;											//generate a singal if reach 0.5 sec
unsigned char counter = 0;							//a counter variable from  0 to 15
unsigned char alphabet = 'A';						//set the fist alpahbet = A
unsigned char received_alphabet;					//the received alphabet from Port B
unsigned char xdata *ptr = 0x00; 					//declare a pointer point to external 8255A

// Function Prototype------------------------------------------------------------------------------------
void initialize (void); 							//initialize the serial port	
void set_baudrate (unsigned int baudrate);			//setup the baud rate
void initialize_t2(); 								//initialize timer2 for interrupt for every 0.5 sec

// Main Program------------------------------------------------------------------------------------------
void main()
{
	AUXR |= 0x02;  									//set up external memory access to 8255 
   	initialize();  									//initialize serial port    
   	initialize_t2();								//initialize timer 2
   	ptr[0x9003] = 0x83;								//set control register = 10000011 = 83H 
    while(1)									   	//repeat main forever
	{
		if(signal) 									//0.5 sec time delay is generated 
		{  		   
		   	//Part 1
			ptr[0x9000] = alphabet; 				//send "A" to Port A for output
		   	alphabet++;								//count from "A" to "Z"
		   	if(alphabet > 'Z' && alphabet < 'a')	//finish from "A" to "Z" 
				alphabet = 'a';						//jump to lower alphabet "a" 

			else if(alphabet > 'z')					//finish from "a" to "z"
		   	    	alphabet = 'A';					//start upper alphabet "A" again

		   	received_alphabet = ptr[0x9001];  		//receive the alphabet from Port B
		   	SBUF = received_alphabet;  				//display the received alphabet to HyperTerminal

			//Part 2
			ptr[0x9002] = counter << 4;				//send the current number to Upper Port C for output
		   	counter++;								//increment counter
		   	counter = counter % 16;					//make sure the counter will not over count
		   	P1 = 0x0F & ptr[0x9002];  		   				//receive the number from Lower Port C to display 
													//on LEDs 
			
			while(TI == 0);							//wait until the byte is sent
			TI = 0;	  								//clear flag

		   	signal = 0;	 							//clear signal
		}										
	}  
}


// Function Definitions----------------------------------------------------------------------------------
void t2_ISR (void) interrupt 5						//interrupt service routine for timer 2
{
	TF2 = 0; 										//clear interrupt flag	  		  	
	timer++;  										//increment the timer
	timer %= 10; 									//make sure the timer will not over count
	if(timer == 0)									//finish counting from 0-9
		signal = 1;									//0.5 sec is reached  
}


void initialize (void)
{
	set_baudrate (9600); 							//set baurd rate to 9600 

	EA = 0;            								//disable all interrupts 
	SM0 = 0; 
	SM1 = 1;	 									//serial mode 1, 1 stop bit, 1 start bit								
	SM2 = 0;
	REN = 1;										//receive enable: allow 8051 transfer & receive data 
	TI = 0;											//clear transmit interrupt flag
	RI = 0;											//clear receive interrupt flag
	ES = 1;											//enable serial interrupts	 
	EA = 1;            								//enable interrupts 
}


void set_baudrate (unsigned int baudrate)
{
	EA = 0;           								//disable all interrupts 
	TI = 0;			 								//clear transmit interrupt flag
	TR1 = 0;	 									//stop timer 1 
	ET1 = 0;	   									//disable timer 1 interrupt 			   
	PCON |= 0x80;									//make baudrate x2 to get the full period 
	TMOD &= ~0xF0;									//clear timer 1 mode bits
	TMOD |= 0x20;									//timer 1, mode2 
	TH1 = (unsigned char) (256 - (XTAL / (16L * 12L * baudrate))); 		
													//formula for getting TH1 value									
  	TR1 = 1;										//timer 1 runs
	EA = 1;              							//enable all interrupts 
}


void initialize_t2()
{
	T2CON = 0x00; 									//set timer2 to auto-reload mode
    ET2 = 1;    									//enable timer 2 interrupts                
	RCAP2L = 0x00;	 								//set the auto-reload value to make the interrupt 
	RCAP2H = 0x4C;									//occur in every 0.5s
	TR2 = 1; 										//timer 2 runs 
	EA = 1;  										//enable interrupt        
}