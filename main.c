/*
 * Lab 2 Question 3.c
 *
 * Created: 9/24/2021 12:08:15 PM
 * Author : xingn
 */ 

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

ISR(TIMER1_CAPT_vect)
{
		// Toggle PB5
		PORTB ^= (1<<PORTB5);
		
		// Toggle rising vs falling edge 
		TCCR1B ^= (1<<ICES1);	
}

void Initialize()
{
	// Disable global interrupts
	cli();
	
	// Set PB5 pin 13 to be output pin
	DDRB  |= (1<<DDB5);
		
	// Set up PB0 pin 8 to be input pin and monitor state
	DDRB &= ~(1<<DDB0);
		
	TIMSK1 |= (1<<OCIE1A); //Enable timer capture interrupt
	TIMSK1 |= (1<<ICIE1);// Enable interrupts 
	TCCR1B |= (0<<ICES1); //Look for falling edge
		
	// Enable global interrupts
	sei();
}

int main(void)
{
	Initialize();
	while(1);
}

