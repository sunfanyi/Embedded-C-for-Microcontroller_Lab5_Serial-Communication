#include <xc.h>
#include "interrupts.h"
#include "serial.h"

/************************************
 * Function to turn on interrupts and set if priority is used
************************************/
void Interrupts_init(void)
{
	// turn on global interrupts, peripheral interrupts and the interrupt source 
    PIE4bits.RC4IE=1;	//receive interrupt
    INTCONbits.GIEL = 1;  // turn on peripheral interrupts
    INTCONbits.GIE = 1;  // turn on global interrupts 
}


/************************************
 * High priority interrvoid Interrupts_init(void)
************************************/
void __interrupt(high_priority) HighISR()
{
    // receiver interrupt
    if (PIR4bits.RC4IF) {
        LATHbits.LATH3 = !LATHbits.LATH3;
        putCharToRxBuf(RC4REG);
    }
    // transmitter interrupt
//    if (PIR4bits.TX4IF) {
//        LATHbits.LATH3 = !LATHbits.LATH3;
//        PIR4bits.TX4IF = 0;  // clear the flag
//    }
}