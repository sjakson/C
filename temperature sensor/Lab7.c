//Tin Fai Hui (Samuel)
//CECS 347
//Lab 7
//12/6/2006


#include <REG52a.H>									//use reg52.h for pre-defined SFR	
#define XTAL 11059200								//crystal oscillator frequency = 11.0592MHz


//Initialize Variables-----------------------------------------------------------------------------------
unsigned char timer = 0; 	  						//a timer variable that count up to 0.5 sec
bit signal;											//generate a singal if reach 0.5 sec
sbit CS = P2^4;
sbit RD = P2^5;										//declare a bit for Read
sbit WR = P2^6;										//declare a bit for Write
sbit INTR = P2^7;									//declare a bit for end conversation
unsigned char temp;									//the received temperature from ADC0804

// Function Prototype------------------------------------------------------------------------------------
void initialize (void); 							//initialize the serial port	
void set_baudrate (unsigned int baudrate);			//setup the baud rate
void initialize_t2(); 								//initialize timer2 for interrupt for every 0.5 sec
void delay(unsigned int value);						//delay function
void display(int value);

// Main Program------------------------------------------------------------------------------------------
void main()
{
	initialize();									//initialize serial port
	initialize_t2();								//initialize timer 2	
	INTR = 1;										//make INTR and input
	RD = 1;											//set RD high
	WR = 1;											//set WR high
	P0 = 0xFF;										//P0 as input					
	CS = 0;											//chip select
				
	while (1)
	{
			WR = 0;									//send WR pulse
			delay(500);								//delay
			WR = 1;
			while (INTR == 1);						//wait for end of conversation
			INTR = 1;
			RD = 0;									//send RD pulse				
			temp = P0;								//read data from ADC0804				
			display(temp);							//display the temperature into HyperTerminal
			RD = 1;
	}	   
}


// Function Definitions----------------------------------------------------------------------------------
void SerialIO_int(void) interrupt 4 using 2
{
   //serial port interrupt service routine
}


void t2_ISR (void) interrupt 5 using 3						//interrupt service routine for timer 2
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


void delay(unsigned int value)
{
	unsigned int i;
	for(i = 0; i < value; i++);
}


void display(int value)
{
	int d1,d2,d3,d4;
	int x = value;
	x = (int)(((double)value * (double) 5 / (double)256) * (double)1000);
													 //convert the value into F degree	
	P1	= value;

	d4 = x /1000;									//display the value
	x -= d4 * 1000;

	d3 = (x) /100;
	x -= d3 * 100;

	d2 = (x) /10;
	x -= d2 * 10;

	d1 =  x;
	
	SBUF = (unsigned char)(48 + d4);
	delay(100);
	
	SBUF =  (unsigned char)(48 + d3);	
	delay(100);
	
	SBUF =  (unsigned char)(48 + d2);
	delay(100);

	SBUF = '.';
	delay(100);

	SBUF =  (unsigned char)(48 + d1);
	delay(100);	
	
	SBUF = 'F';
	delay(100);	

	SBUF = '\r';
	delay(100); 
}
