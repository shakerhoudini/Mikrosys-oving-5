//Oppgave 2 


//USART uses RXSIF (Start Bit Enable) & RXCIF (Recive Complete Interrupt) as interrupts as well as
//TXCIF, DREIF, ISFIF on pins 7 - 3 in USARTn.STATUS register



ISR(USART3_RXC_vect){ 
	// we use USART3_RXC_vect as interrupt vector, from page 72 for RXD for USART3
	//going to use RXCIF as interrupt with RXSIF since they share interrupt line
	//we have it enabled via USART3.CTRLB register on bit 7 while keeping bit 4 low
	//aka keeping RXSIF disabled
	;
}
