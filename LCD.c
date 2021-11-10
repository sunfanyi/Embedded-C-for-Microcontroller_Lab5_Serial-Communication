#include <xc.h>
#include <stdio.h>
#include "LCD.h"

/************************************
 * Function to toggle LCD enable bit on then off
 * when this function is called the LCD screen reads the data lines
************************************/
void LCD_E_TOG(void)
{
	//turn the LCD enable bit on
    LCD_E = 1;
	__delay_us(2); //wait a short delay
	//turn the LCD enable bit off again
    LCD_E = 0;
}

/************************************
 * Function to set the 4-bit data line levels for the LCD
************************************/
void LCD_sendnibble(unsigned char number)
{
	//set the data lines here (think back to LED array output)
    if (number & 0b1000) {LCD_DB7 = 1;} else {LCD_DB7 = 0;}
    if (number & 0b0100) {LCD_DB6 = 1;} else {LCD_DB6 = 0;}
    if (number & 0b0010) {LCD_DB5 = 1;} else {LCD_DB5 = 0;}
    if (number & 0b0001) {LCD_DB4 = 1;} else {LCD_DB4 = 0;}
    
    LCD_E_TOG();			//toggle the enable bit to instruct the LCD to read the data lines
    __delay_us(5);      //Delay 5uS
}


/************************************
 * Function to send full 8-bit commands/data over the 4-bit interface
 * high nibble (4 most significant bits) are sent first, then low nibble sent
************************************/
void LCD_sendbyte(unsigned char Byte, char type)
{
    // set RS pin whether it is a Command (0) or Data/Char (1) using type argument
    if (type == 0) {LCD_RS = 0;} else if (type == 1) {LCD_RS = 1;} 
    // send high bits of Byte using LCDout function
    // send low bits of Byte using LCDout function
	unsigned char msb;
    unsigned char lsb;
    msb = (Byte >> 4);
    LCD_sendnibble(msb);
    lsb = (Byte & 0b00001111);
    LCD_sendnibble(lsb);
    __delay_us(50);               //delay 50uS (minimum for command to execute)
}

/************************************
 * Function to initialise the LCD after power on
************************************/
void LCD_Init(void)
{
    __delay_ms(10);  // ensure power on
    //Define LCD Pins as Outputs and
    //set all pins low (might be random values on start up, fixes lots of issues)
    TRISEbits.TRISE1 = 0;	
    TRISEbits.TRISE3 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    // waste hours just because these two lines are missing...
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC6 = 0;
	//set initial output LAT values (they may have random values when powered on)
    LCD_DB7 = 0;	
    LCD_DB6 = 0;
    LCD_DB5 = 0;
    LCD_DB4 = 0;
    
    // Initialisation sequence code
	// follow the sequence in the GitHub Readme picture for 4-bit interface.
	// first Function set should be sent with LCD_sendnibble (the LCD is in 8 bit mode at start up)
	// after this use LCD_sendbyte to operate in 4 bit mode
    // function set 1
    LCD_sendnibble(0b0011);
    __delay_us(50);
    // function set 2
    LCD_sendbyte(0b00101000,0);  // 0x28 enable 5x8 mode for chars 
    // function set 3
    LCD_sendbyte(0b00101000,0);
    // display ON/OFF control
    LCD_sendbyte(0b00001000,0);  // display off, cursor off, blinking off
    // display clear
    LCD_sendbyte(0b00000001,0);  
    __delay_ms(2);
    // entry mode set
    LCD_sendbyte(0b00000110,0);  // 0x06 Auto Increment cursor, shift display off
	//remember to turn the LCD display back on at the end of the initialisation (not in the data sheet)
    LCD_sendbyte(0b00001100,0);  // display on, cursor off, blinking off
}

/************************************
 * Function to set the cursor to beginning of line 1 or 2
************************************/
void LCD_setline (char line)
{
    //Send 0x80 to set line to 1 (0x00 ddram address)
    if (line == 1) {
        LCD_sendbyte(0x80,0);  //  Cursor at home position
    } else if (line == 2) {
        LCD_sendbyte(0xC0,0);
    }
    //Send 0xC0 to set line to 2 (0x40 ddram address)
}

/************************************
 * Function to send string to LCD screen
************************************/
void LCD_sendstring(char *string)
{
	//code here to send a string to LCD using pointers and LCD_sendbyte function
    while (*string) {LCD_sendbyte(*string++,1);}
}

/************************************
 * Function to send string to LCD screen
************************************/
void LCD_scroll(void)
{
	//code here to scroll the text on the LCD screen
    __delay_ms(300);
    LCD_sendbyte(0b00011000,0); // shift display, shift left
}

/************************************
 * Function takes a ADC value and works out the voltage to 2 dp
 * the result is stored in buf as ascii text ready for display on LCD
 * Note result is stored in a buffer using pointers, it is not sent to the LCD
************************************/
void ADC2String(char *buf, unsigned int ADC_val)
{
    //code to calculate the integer and fractions part of a ADC value
    unsigned int int_part = ADC_val/77; //255/77=3.3, get integer part of voltage
    unsigned int frac_part = ADC_val*100/77 - int_part*100; //get fractional part of voltage
    
    // the following method eliminates error but potentially cause value overflow issue
//    unsigned long int_part = ADC_val * 33 / 255 / 10;
//    unsigned long frac_part = (ADC_val * 100 * 33 / 255 / 10) - int_part * 100;
	// and format as a string using sprintf (see GitHub readme)
    sprintf(buf,"%d.%02dV", int_part, frac_part); //put string in buffer array address, in 3 figures with leading 0's
}