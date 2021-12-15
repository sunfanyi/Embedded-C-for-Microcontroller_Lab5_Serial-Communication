#include <xc.h>
#include "interrupts.h"
#include "serial.h"

/************************************
 * Function to turn on interrupts and set if priority is used
************************************/
void Interrupts_init(void)
{
	// turn on global interrupts, peripheral interrupts and the interrupt source 
    PIE4bits.RC4IE = 1;	//receive interrupt
    PIE4bits.TX4IE = 0;
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
    // transmitter interrup
    if (PIR4bits.TX4IF && (PIE4bits.TX4IE == 1)){ //write buffer activated, TX4REG is empty
        TX4REG = getCharFromTxBuf();
        if (!isDataInTxBuf()) {PIE4bits.TX4IE = 0;}
    }
}