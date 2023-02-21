//Når knappen på PB2 trykkes, vil koden umiddelbart starte en løkke som sjekker om knappen er sluppet.
//Dette vil føre til at programmet blir hengende i løkken og ikke kan utføre andre oppgaver (for eksempel å bytte LED-tilstanden).

//Inkludering
#define F_CPU 4000000UL
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

volatile uint8_t pressCount = 0;			//volitale since it used in an interrupt

int main(void)
{
	PORTE.DIRSET = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm;		//Setter som output
	PORTB.DIRSET = PIN3_bm;                                     //Setter onboard led som output 
	PORTB.PIN2CTRL = PORT_PULLUPEN_bm |	PORT_ISC_FALLING_gc;	//set button as input on press down and enable interrupt flag call
	
	sei();							// enable global interrupt
	
	while (1)
	{
		if(pressCount > 2){
			PORTB.OUTTGL = PIN3_bm; //toggle onboard led 
			pressCount = 0;			//reset counter
		}
	}
}

ISR(PORTB_PORT_vect){
	PORTE.OUTTGL = 0b00000111;			//toggle output
	do{
		;
	 } 
	while (!(PORTB.IN & PIN2_bm));	//wait for button relase
	
	pressCount++;						//adds button pressed cause of how the function is constructed
	PORTB.INTFLAGS = PIN2_bm;			//disable interrupt
}
