#include <LPC21xx.H> // LPC21xx definitions
//#define SPI_OK 0 // transfer ended no errors
//#define SPI_BUSY 1 // transfer busy
//#define SPI_ERROR 2 // SPI error
//static unsigned char state; // State of SPI driver
static unsigned char spiBuf[4]; // SPI data pointer
static unsigned char *msg; // pointer to SPI data buffer
static unsigned char count; // nr of bytes sent/received
static unsigned char dummy; // value for dummy reads to clear status register
static unsigned char SO_val; // slave output value
int ch;

static void SPI_Init(void);
static void EEPROM_Write(unsigned char add, unsigned char val);
unsigned char EEPROM_Read(unsigned char add);
int putchar (int ch);

int main(void)									
{
	SPI_Init();

	EEPROM_Write(0x00,0xE); // write to SPI chip
	SO_val = EEPROM_Read(0x00); // read from SPI chip

	putchar(SO_val); //send the value from the UART to the screen
	
}

static void SPI_Init(void)
{
	IODIR0 |= 0x00002000; // P0.13 set as output 
	IOSET0 = 0x00002000; //  P0.13 = 1
	PINSEL0 |= 0x00005500; // configure SPIO pins
	S0SPCCR = 11; // SCK = 2.72 MHz, counter > 8 and even
	S0SPCR = 0x28; // CPHA = 1, CPOL = 0, master mode/MSB 1st
}


static void EEPROM_Write(unsigned char add, unsigned char SI_val)
{
   spibuf[0] = 0x03; // write latch enable instruction
   spiBuf[1] = 0x01; // write instruction
   spiBuf[2] = add;	 // address of instruction
   spiBuf[3] = SI_val;
   msg = spiBuf;
   count = 3;
   
   IOCLR0 =  0x00002000; // CS low 
   S0SPDR = *msg; // transfer write enable instruction
   while (!(S0SPSR & 0x80)); // wait for SPIF transfer complete flag to set
   dummy = S0SPSR; // dummy read to clear flag
   *msg++; // increment pointer
   IOSET0 =  0x00002000; // CS high to complete write enable instruction 

   IOCLR0 =  0x00002000; // CS low 
   while (count > 0)
   {
   S0SPDR = *msg; // transfer byte
   while (!(S0SPSR & 0x80)); // wait for SPIF transfer complete flag to set
   count--; // decrement counter (number of bytes left to write)
   dummy = S0SPSR; // dummy read to clear flag
   *msg++; // increment pointer
   }

   IOSET0 =  0x00002000; // CS high

}


unsigned char EEPROM_Read(unsigned char add)
{

   unsigned char val;
   spiBuf[0] = 0x03; // read instruction
   spiBuf[1] = add;	 // address of instruction
   msg = spiBuf;
   count = 2;
   IOCLR0 =  0x00002000; // CS low 

   while (count > 0)
   {
   S0SPDR = *msg; // transfer byte
   while (!(S0SPSR & 0x80)); // wait for SPIF transfer complete flag to set
   count--; // decrement counter (number of bytes left to write)
   dummy = S0SPSR; // dummy read to clear flag
   *msg++; // increment pointer
   }

	val = S0SPDR; // read the slave output value
    
	IOSET0 =  0x00002000; // CS high

	return val;


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



