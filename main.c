/*
 * Lab 2 Question 2.c
 *
 * Created: 9/24/2021 10:57:14 AM
 * Author : xingn
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL

#include <util/delay.h>


int main(void)
{
	// Disable global interrupts
	cli();
	
    /* Replace with your application code */
    while (1) 
    {
		// Set PB5 pin 13 to be output pin
		DDRB  |= (1<<DDB5);
		
		// Set up PB0 pin 8 to be input pin and monitor state
		DDRB &= ~(1<<DDB0);
		
		if (PINB & (1<<PINB0)) {
			// Turn PB5 on
			PORTB &= ~(1<<PORTB5);
			
		} else {
			// Turn PB5 off
			PORTB |= (1<<PORTB5);
			
		}
		sei();
    }
}

