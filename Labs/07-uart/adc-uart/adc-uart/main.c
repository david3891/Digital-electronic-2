/***********************************************************************
 * 
 * Analog-to-digital conversion with displaying result on LCD and 
 * transmitting via UART.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-Present Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "lcd.h"            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for conversion function
#include "uart.h"           // Peter Fleury's UART library

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Use Timer/Counter1 and start ADC conversion four times 
 *           per second. Send value to LCD and UART.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Initialize LCD display
    lcd_init(LCD_DISP_ON);
    lcd_gotoxy(1, 0); lcd_puts("value:");
    lcd_gotoxy(3, 1); lcd_puts("key:");
    lcd_gotoxy(8, 0); lcd_puts("a");    // Put ADC value in decimal
    lcd_gotoxy(13,0); lcd_puts("b");    // Put ADC value in hexadecimal
    lcd_gotoxy(8, 1); lcd_puts("c");    // Put button name here

    // Configure ADC to convert PC0[A0] analog value
    // Set ADC reference to AVcc
    ADMUX |= (1<<REFS0);
    ADMUX &= ~(1<<REFS1);
    // Set input channel to ADC0
    ADMUX &= ~(1<<MUX0);
    ADMUX &= ~(1<<MUX1);
    ADMUX &= ~(1<<MUX2);
    ADMUX &= ~(1<<MUX3);
    // Enable ADC module
    ADCSRA |= (1<<ADEN);
    // Enable conversion complete interrupt
    ADCSRA |= (1<<ADSC);
    // Set clock prescaler to 128
    ADCSRA |= (1<<ADPS2);
    ADCSRA |= (1<<ADPS1);
    ADCSRA |= (1<<ADPS0);
    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Set prescaler to 262 ms and enable overflow interrupt
    TIM1_overflow_262ms()
	TIM1_overflow_interrupt_enable();
    // Initialize UART to asynchronous, 8N1, 9600
    uart_init(UART_BAUD_SELECT(9600,16000000));
    // Enables interrupts by setting the global interrupt mask
    sei();
    ADCSRA |= (1<<ADIE);

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Use single conversion mode and start conversion four times
 *           per second.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
    ADCSRA |= (1<<ADSC);    // start ADC
}

/**********************************************************************
 * Function: ADC complete interrupt
 * Purpose:  Display value on LCD and send it to UART.
 **********************************************************************/
ISR(ADC_vect)
{
	uint16_t value = 0;
	char lcd_string[4] = "0000";
	value = ADC;	  // Copy ADC result to 16-bit variable
	lcd_gotoxy(8,0);  //vymazani prvni hodnoty
	lcd_puts("    ");
	//vlozeni nove hodnoty na display
	itoa(value, lcd_string, 10);  // prevod desitkove soustavy na string
	lcd_gotoxy(8,0);
	lcd_puts(lcd_string);
	// poslani na UART
	uart_puts(lcd_string);
	uart_puts(" ");
	//vymazani prvni hodnoty
	lcd_gotoxy(13,0);
	lcd_puts("    ");
	//nova hodnota na display
	//hodnota na display v hexadecimalni
	itoa(value, lcd_string, 16);    // prevod hexadecimalni soustavy na string
	lcd_gotoxy(13,0);
	lcd_puts(lcd_string);
	//zobrazit po stisku tlacitka
	lcd_gotoxy(8,1);
	lcd_puts("    ");
	lcd_gotoxy(12,1);
	lcd_puts("    ");
	
	lcd_gotoxy(8, 1);                // cosi
	itoa(value, lcd_string, 10);     // prevod desitkove soustavy na string

	if (value>1000)                { lcd_puts("NONE");}
	if ((value>600)&&(value<1000)) { lcd_puts("SELECT");}
	if ((value>350)&&(value<450))  { lcd_puts("LEFT");}
	if ((value>200)&&(value<270))  { lcd_puts("DOWN");}
	if ((value>5)&&(value<120))    { lcd_puts("UP");}
	if (value==0)                  { lcd_puts("RIGHT");}
	;
}