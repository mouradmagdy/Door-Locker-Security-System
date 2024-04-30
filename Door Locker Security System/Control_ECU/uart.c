/*
 * uart.c
 *
 *  Created on: Mar 22, 2024
 *      Author: Mourad
 */

#include"uart.h"
#include<avr/io.h>
#include"common_macros.h"

void UART_init(const UART_ConfigType* Config_Ptr){
	uint16 ubrr_value=0;
	/* U2X = 1 for double transmission speed*/
	UCSRA=(1<<U2X);

	/*****************************UCSRB Description**********************
	 * RXCIE=0 Disable USART RX Complete Interrupt Enable
	 * TXCIE=0 Disable USART TX Complete Interrupt Enable
	 * UDRIE=0 Disable UART Data Register Empty Interrupt Enable
	 * RXEN=1  Receiver Enable
	 * TXEN=1  Transmitter Enable
	 * UCSZ2=0 For 8-bit data mode
	 * RXB8 & TXB8 not used for 8-bit data mode
	 ********************************************************************/
	UCSRB=(1<<RXEN)|(1<<TXEN);
	/* Adjusting UCSZ2 bit while preserving the others*/
	UCSRB=(UCSRB&0xFB)|(Config_Ptr->bit_data&0x04);

	/******************************UCSRC Description**********************
	 * URSEL = 1 The URSEL must be one when writing the UCSRC
	 * UMSEL = 0 Asynchronous operation
	 * UPM1:0= 00 Disable parity bit
	 * USBS  = 0 one stop bit
	 * UCSZ1:0= 11 for 8-bit data mode
	 * UCPOL =0 used with synchronous operation only

	 **********************************************************************/
	/* URSEL bit must be one when writing the register*/
	/* Adjusting UCSZ1 & UCSZ0 bit while preserving the others*/
	UCSRC=(UCSRC&0x79)|(Config_Ptr->bit_data&0x83);
	/* Adjusting USBS bit while preserving the others*/
	UCSRC=(UCSRC&0x77)|(Config_Ptr->stop_bit&0x80);
	/* Adjusting UPM1:0 while preserving the others */
	UCSRC=(UCSRC&0x4F)|(Config_Ptr->parity&0xB0);

	/* calculate the UBRR register value*/
	ubrr_value=(uint16)(((F_CPU/(Config_Ptr->baud_rate*8UL)))-1);

	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last for bits in UBRRH*/
	UBRRH=ubrr_value>>8;
	UBRRL=ubrr_value;

}

void UART_sendByte(const uint8 data){
	/*
	 * UDRE flag is set when the TX buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until the flag is set to 1
	 */

	while(BIT_IS_CLEAR(UCSRA,UDRE));

	/*
	 * Put the required data in the UDR register and it also clears the UDRE flag as
	 * the UDR register is not empty now */

	UDR=data;

}

uint8 UART_receiveByte(void){
	/* RXC flag is set when the UART receive data so wait until this flag is set to 1*/
	while(BIT_IS_CLEAR(UCSRA,RXC));
	return UDR;
}

void UART_sendString(const uint8 *Str){
	uint8 i=0;
	while(Str[i]!='\0'){
		UART_sendByte(Str[i]);
		i++;
	}
}

void UART_receiveString(uint8 *Str){
	uint8 i=0;
	Str[i]=UART_receiveByte();
	while(Str[i]!='#'){
		i++;
		Str[i]=UART_receiveByte();
	}
	/* After receiving the whole string plus the '#' replace the '#' with '\0'*/
	Str[i]='\0';
}





