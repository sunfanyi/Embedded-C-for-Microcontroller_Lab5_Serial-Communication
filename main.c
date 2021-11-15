// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include <stdio.h>
#include "LCD.h"
#include "serial.h"
#include "ADC.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void) {
    LCD_Init();
    initUSART4();
    ADC_init();
    
    unsigned int ADC_val;
    char buf[0];
    
    while(1){
        ADC_val = ADC_getval();
        ADC2String(buf, ADC_val);
        
        LCD_sendbyte(0b00000001,0); // display clear
        __delay_ms(2);
        LCD_sendstring(buf);
        
        sendStringSerial4(buf);
        sendStringSerial4("   ");
        __delay_ms(1000);
    }
}

