	  /********************************************************/
/* Examples Program For "Olimex ARM7 USB-LPC2148" */
/* Target MCU : Philips ARM7-LPC2148 */
/* : X-TAL : 12.00 MHz */
/* : Run Speed 60.00 MHz (With PLL) */
/* : PLL Setup = M(5),P(2) */
/* : VPB Clock = (1/4)CPU Clock = 15.00 MHz */
/* Keil Editor : uVision3 V3.03a */
/* Compiler : Keil CARM V2.50a */
/* Create By : John Tramel CSULB */
/* Last Update : 1October2008 */
/* Function : Example LED Blink on GPIO0[11:10] */
/********************************************************/
// Connect P0.10&11 to LEDs For Test ON / OFF (Blink)
// Want the two LEDs to blink at different rates
#include "LPC214x.H" // LPC2148 MPU Registers
/* prototype section */
void delay(unsigned long int); // Delay Time Function
int main(void)
{
int holdval = 0x00000C00;
int count1 = 0; // counter for bit 10
int count2 = 0; // counter for bit 11
// xxxx xxxx xxxx xxxx xxxx 11xx xxxx xxxx
IODIR0 = holdval; // Set GPIO-0[11:10] = Outputs
IOSET0 = holdval; // Set GPIO-0[11:10] Outputs Pin(OFF LED)
// Loop Test Output GPIO0.10
while(1) // Loop Continue
{
if (++count1 == 100000) {
IOCLR0 = 0x00000400;
} else
if (count1 == 1000000) {
IOSET0 = 0x00000400;
count1 = 0;
}
if (++count2 == 300000)
IOCLR0 = 0x00000800; else
if (count2 == 900000) {
IOSET0 = 0x00000800;
count2 = 0;
}
}
}
