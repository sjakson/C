// Scott Jackson
// CECS 347
// Midterm #2
// 4/24/08

//C program that reads the lower half of a DIP switch on the external data bus at 
//address 4000Hand writes the value back to the data bus at location 4000H to light 
//up the corresponding LEDs

#include <reg51.h>
#include <absacc.h>

sfr AUXR = 0x8E;

void main(void) {

AUXR = 0x03; //enable ALE to use P0 as an address bus

while(1){

	ACC = XBYTE[0x4000]; //read in data from external memory at location 4000H
	XBYTE[0x4000] = ACC; //write data to external memory location 4000H




}






}