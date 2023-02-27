//Oppgave 2 

//Definisjoner
#define F_CPU 4000000UL
#define USART3_BAUD_RATE(BAUD_RATE)	((float)(64*F_CPU/(16*(float)BAUD_RATE))+0.5)

//Inkludering av bibilotek
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

//Initialisering av funksjoner
void USART3_init(void);
static void USART3_sendChar(char c);
static int USART3_printChar(char c, FILE *stream);
void USART3_sendString(char *str);
uint8_t USART_read();
void UsartInit();

//Initialisering av variabler
volatile uint8_t UsartData[100];			//Bruker volatile siden data vil bli lastet inn fra en interrupt 
volatile uint8_t u_dataIterator = 0;		//iterator for UsartData
volatile bool isEOL = false;                //Boollean variabel
static FILE Usart_stream = FDEV_SETUP_STREAM(USART3_printChar,NULL,_FDEV_SETUP_WRITE);
volatile uint8_t pressCount = 0;			//volitale since it used in an interrupt

int main(void)
{
	UsartInit(); 
	
	USART3.CTRLA = PIN7_bm;					                    //enables RXCIE therefore enables interrupt on RXCIF set to high
	PORTE.DIRSET = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm;		//setter pins som output
	PORTB.PIN2CTRL = PORT_PULLUPEN_bm |	PORT_ISC_FALLING_gc;	//setter button som input på press down og enable interrupt flag call
	
	sei();							// enable global interrupt
	while (1)
	{
		if(pressCount > 2){
			PORTE.OUTTGL = PIN3_bm; //toggle led on pin3
			pressCount = 0;			//resetter counter
		}
		
		if(isEOL){
			uint8_t tempArray[100];				//Temp array for lagring av USART data
			strcpy(tempArray, &UsartData);		//Copy UsartData til TempArray
			printf(tempArray);					//Print Command/Array
			
			isEOL = false;						//Reset End Of Line Flag
			
		}
		
	}
}

//USART bruker RXSIF (Start Bit Enable) & RXCIF (Recive Complete Interrupt) som interrupts også
//TXCIF, DREIF, ISFIF på pins 7 - 3 i USARTn.STATUS register

ISR(USART3_RXC_vect){
	isEOL = false;						//Reset End Of Line Flag
	memset(UsartData,NULL,100);			//Reset UsartData Array
	u_dataIterator = 0;					//Reset UsartData Iterator
	
	cli();	//Start Conversion - Global Interrupt Disable
	
	uint8_t load = USART_read();
	UsartData[u_dataIterator] = load;
	u_dataIterator++;
	
	if(load == 0x0D){
		isEOL = true;
	}
	
	sei();	//Ends Conversion - Global Interrupt Enable
	
}

//Funskjoner knyttet til USART fra tidligere øving

void UsartInit(){
	PORTB.DIRCLR = PIN1_bm; //= PORTB.DIRSET = PIN1_bm;
	PORTB.DIRSET = PIN0_bm;  //= PORTB.DIRCLR = PIN0_bm;
	USART3.BAUD = (uint16_t)USART3_BAUD_RATE(9600);
	USART3.CTRLB |= USART_TXEN_bm | USART_RXEN_bm;
	
	stdout = &Usart_stream;
}

static void USART3_sendChar(char c)
{
	while (!(USART3.STATUS & USART_DREIF_bm))
	{
		;
	}
	USART3.TXDATAL = c;
}
void USART3_sendString(char *str)
{
	for(size_t i = 0; i < strlen(str); i++)
	{
		USART3_sendChar(str[i]);
	}
}

uint8_t USART_read(){
	while(!(USART3.STATUS & USART_RXCIF_bm)){
		;
	}
	uint8_t srt = USART3.RXDATAL;
	//USART3_sendChar(srt);
	return srt;
}

static int USART3_printChar(char c, FILE *stream){
	USART3_sendChar(c);
	return 0;
}
