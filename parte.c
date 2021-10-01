/*
 * GccApplication1.c
 *
 * Created: 9/28/2021 2:34:52 PM
 * Author : xingn
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdbool.h>

#include "uart.h"
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

// volatile int
unsigned long edge1 = 0;
unsigned long signalLen = 0;
unsigned long signalLen_low = 0;
unsigned long ovf_count = 0;
volatile bool High = false;

char String[10];
char String2[25];
char String3[25];

char charDecode[6];

//16 bit timer 65536 steps

unsigned long dotCount = 1966; //937
unsigned long dashCount = 13100; //6249
unsigned long spaceCount = 26000; //12499
// 30 ms: 33.33 Hz -> (16MHz/256 clock prescaler)/(2 * 1 timer prescaler + count + 1)  = 33.33 Hz -> count = 937
// 200 ms: 5 Hz -> (16MHz/256 clock prescaler)/(2* 1 timer prescaler + count + 1) = 5 Hz -> count = 6249
// 400 ms: 2.5 Hz -> (16Mhz/256 clock prescaler)/2* 1 timer prescaler + count + 1) = 2.5 Hz -> count = 12499

ISR(TIMER1_CAPT_vect) 
{
	TCCR1B ^= (1<<ICES1); // Toggle to look for rising or falling after
	
	// Calculate length of the signal
	if(High) {
		signalLen = ICR1 - edge1 + ovf_count * 65535;
	} else {
		signalLen_low = ICR1 - edge1 + ovf_count * 65535;
	}

	// sprintf(String2, "signalLen: %u \n", signalLen);
	// UART_putstring(String2);
	// sprintf(String3, "overflow, ICR1, edge1: %u,  %u,  %u \n \n", ovf_count, ICR1, edge1);
	// UART_putstring(String3);

	edge1 = ICR1; // Save value of edge	
	ovf_count = 0; // Reset overflow counter

	if( ( dotCount <= signalLen && signalLen < dashCount) && High ) {
		// sprintf(String,"dot \n");
		// UART_putstring(String);
		
		// Turn LED On/Off
		PORTB |= (1<<PORTB5);
		_delay_ms(50);
		PORTB &= ~(1<<PORTB5);
		_delay_ms(50);
		
		strcat((char *)charDecode, (char *)"0");
	}
	else if ( (dashCount <= signalLen) && High ) {
		// sprintf(String,"dash \n");
		// UART_putstring(String);
		
		// Turn LED On/Off
		PORTB |= (1<<PORTB4);
		_delay_ms(50);
		PORTB &= ~(1<<PORTB4);
		_delay_ms(50);
		
		strcat((char *)charDecode, (char *)"1");
	}
	else if ( ( spaceCount <= signalLen_low) ) {
		// sprintf(String,"space \n");
		// UART_putstring(String);
		
		if (!strcmp(charDecode,"01")) {
			sprintf(String2,"A");
			UART_putstring(String2);
		} else if (!strcmp(charDecode,"1000")) {
			sprintf(String2,"B");
			UART_putstring(String2);
		} else if (!strcmp((char *)charDecode,(char *)"1010")) {
			sprintf(String2,"C");
			UART_putstring(String2);	
		} else if (!strcmp((char *)charDecode,(char *)"100")) {
			sprintf(String2,"D");
			UART_putstring(String2);	
		} else if (!strcmp((char *)charDecode,(char *)"0")) {
			sprintf(String2,"E");
			UART_putstring(String2);
		} else if (!strcmp((char *)charDecode,(char *)"0010")) {
			sprintf(String2,"F");
			UART_putstring(String2);
		} else if (!strcmp((char *)charDecode,(char *)"110")) {
			sprintf(String2,"G");
			UART_putstring(String2);	
		} else if (!strcmp((char *)charDecode,(char *)"0000")) {
			sprintf(String2,"H");
			UART_putstring(String2);
		} else if (!strcmp((char *)charDecode,(char *)"00")) {
			sprintf(String2,"I");
			UART_putstring(String2);
		} else if (!strcmp((char *)charDecode,(char *)"0111")) {
			sprintf(String2,"J");
			UART_putstring(String2);
		} else if (!strcmp((char *)charDecode,(char *)"101")) {
			sprintf(String2,"K");
			UART_putstring(String2);
		} else if (!strcmp((char *)charDecode,(char *)"0100")) {
			sprintf(String2,"L");
			UART_putstring(String2);
		} else if (!strcmp((char *)charDecode,(char *)"11")) {
			sprintf(String2,"M");
			UART_putstring(String2);
		} else if (!strcmp((char *)charDecode,(char *)"10")) {
			sprintf(String2,"N");
			UART_putstring(String2);
		} else if (!strcmp((char *)charDecode,(char *)"111")) {
			sprintf(String2,"O");
			UART_putstring(String2);
		} else if (!strcmp((char *)charDecode,(char *)"0110")) {
			sprintf(String2,"P");
			UART_putstring(String2);
		} else if (!strcmp((char *)charDecode,(char *)"1101")) {
			sprintf(String2,"Q");
			UART_putstring(String2);
		} else if (!strcmp((char *)charDecode,(char *)"010")) {
			sprintf(String2,"R");
			UART_putstring(String2);
		} else if (!strcmp((char *)charDecode,(char *)"000")) {
			sprintf(String2,"S");
			UART_putstring(String2);
		} else if (!strcmp((char *)charDecode,(char *)"1")) {
			sprintf(String2,"T");
			UART_putstring(String2);
		} else if (!strcmp((char *)charDecode,(char *)"001")) {
			sprintf(String2,"U");
			UART_putstring(String2);
		} else if (!strcmp((char *)charDecode,(char *)"0001")) {
			sprintf(String2,"V");
			UART_putstring(String2);
		} else if (!strcmp((char *)charDecode,(char *)"011")) {
			sprintf(String2,"W");
			UART_putstring(String2);
		} else if (!strcmp((char *)charDecode,(char *)"1001")) {
			sprintf(String2,"X");
			UART_putstring(String2);
		} else if (!strcmp((char *)charDecode,(char *)"1011")) {
			sprintf(String2,"Y");
			UART_putstring(String2);
		} else if (!strcmp((char *)charDecode,(char *)"1100")) {
			sprintf(String2,"Z");
			UART_putstring(String2);
		}
		memset(charDecode, 0, 6); // Reset Char array
	}

	High = !High; // Toggle (pressed/not pressed) monitor
	_delay_ms(5); // Debounce logic
	
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
	
	// Set output pins 
	DDRB |= (1<<DDB5); //Pin 13
	DDRB |= (1<<DDB4); //Pin 12
	
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

