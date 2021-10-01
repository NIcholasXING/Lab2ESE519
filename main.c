/*
 * GccApplication1.c
 *
 * Created: 9/28/2021 2:34:52 PM
 * Author : xingn
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL

#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdbool.h>

#include "uart.h"
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

volatile int edge1 = 0;
volatile int signalLen = 0;
volatile int ovf_count = 0;
volatile bool High = false;
char String[10];
char String2[25];
char String3[25];

//16 bit timer 65536 steps

int dotCount = 937;
int dashCount = 6249;
int spaceCount = 12499;
// 30 ms: 33.33 Hz -> (16MHz/256 clock prescaler)/(2 * 1 timer prescaler + count + 1)  = 33.33 Hz -> count = 937
// 200 ms: 5 Hz -> (16MHz/256 clock prescaler)/(2* 1 timer prescaler + count + 1) = 5 Hz -> count = 6249
// 400 ms: 2.5 Hz -> (16Mhz/256 clock prescaler)/2* 1 timer prescaler + count + 1) = 2.5 Hz -> count = 12499

ISR(TIMER1_CAPT_vect) 
{
	signalLen = ICR1 - edge1;
	TCCR1B ^= (1<<ICES1); // Toggle to look for rising or falling after
	
	// Calculate length of the signal
	signalLen = ICR1 - edge1 + ovf_count * 65535;


	// sprintf(String2, "signalLen: %u \n", signalLen);
	// UART_putstring(String2);
	// sprintf(String3, "overflow, ICR1, edge1: %u,  %u,  %u \n \n", ovf_count, ICR1, edge1);
	// UART_putstring(String3);


	edge1 = ICR1; // Save value of edge	
	ovf_count = 0; // Reset overflow counter

	if( ( dotCount <= signalLen && signalLen < dashCount) && High ) {
		sprintf(String,"dot \n");
		UART_putstring(String);
	}
	else if ( (dashCount <= signalLen) && High ) {
		sprintf(String,"dash \n");
		UART_putstring(String);
	}
	else if ( ( spaceCount <= signalLen ) && !High ) {
		sprintf(String,"space \n");
		UART_putstring(String);
	}

	High = !High; // Toggle (pressed/not pressed) monitor
	_delay_ms(50); // Debounce logic
	
	// TIFR1 |= (1<<ICF1) // Clear input capture flag
}

ISR(TIMER1_OVF_vect)
{
	ovf_count++;
}

void Initialize()
{
	cli(); // Disable global interrupts
	
	// Prescale 16Mhz system clock to 62.5 Khz
	CLKPR |= (1<<CLKPCE); // Enable changes to CLKPS bits
	CLKPR |= (1<<CLKPS3); // Divide by 256

	// GPIO Pins Setup
	DDRB &= ~(1<<DDB0); // Set up PB0 pin 8 to be input pin and monitor state
	
	// Timer 1 Setup - Set Timer clock to be internal div by 1 (no prescale)
	TCCR1B |= (1<<CS12); // CS10 debug
	TCCR1B &= ~(1<<CS11);
	TCCR1B &= ~(1<<CS10); // CS12 debug
	
	// Timer 1 Setup - Set Timer clock to normal
	TCCR1A &= ~(1<<WGM10);
	TCCR1A &= ~(1<<WGM11);
	TCCR1A &= ~(1<<WGM12);
	TCCR1A &= ~(1<<WGM13);
	
	// Looking for a rising edge 
	TCCR1B |= (1<<ICES1);
	
	// Looking for a Timer 1 Overflow
	TIFR1 |= (1<<TOV1);
	
	// Enable input capture interrupt
	TIMSK1 |= (1<<ICIE1);
	
	// Enable Timer 1 Overflow Interrupt
	TIMSK1 |= (1<<TOIE1);
	
	// Enabled global interrupts
	sei();
	
	
	// Clear interrupt/input capture flag
	// TIFR1 |= (1<<ICF1);
	
	// Enable Interrupt
	// TIMSK1 |= (1<<OCIE1A);
	UART_init(BAUD_PRESCALER);
}

int main(void)
{
	Initialize();
		
    while (1) 
    {
    }
}

