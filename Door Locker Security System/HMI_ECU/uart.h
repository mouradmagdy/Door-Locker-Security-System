/*
 * uart.h
 *
 *  Created on: Mar 22, 2024
 *      Author: Mourad
 */

#ifndef UART_H_
#define UART_H_

#include"std_types.h"

/* enum to specify the bit data in the UART*/
typedef enum{
	FIVE_BIT_MODE=0,
	SIX_BIT_MODE=1,
	SEVEN_BIT_MODE=2,
	EIGHT_BIT_MODE=3,
	NINE_BIT_MODE=7,

}UART_BitData;


/* enum to specify even or odd parity in the UART*/
typedef enum{
	EVEN_PARITY=2,
	ODD_PARITY=3,
}UART_Parity;

typedef enum{
	ONE_STOP_BIT,
	TWO_STOP_BIT,
}UART_StopBit;

typedef enum{
	RATE_ONE=2400,
	RATE_TWO=4800,
	RATE_THREE=9600,
	RATE_FOUR=14400,
}UART_BaudRate;

typedef struct{
	UART_BitData bit_data;
	UART_Parity parity;
	UART_StopBit stop_bit;
	UART_BaudRate baud_rate;
}UART_ConfigType;


/* Description:
 * Function responsible for initialization of UART device by:
 * - Setup the frame format like number of data bits, parity bit type and number of stop bits
 * - Enable the UART.
 * - Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType* Config_Ptr);

/*
 * Description:
 * Function responsible for sending byte to another UART device.
 */

void UART_sendByte(const uint8 data);

/*
 * Description:
 * Function responsible for receiving byte from another UART device.
 */

uint8 UART_receiveByte(void);

/*
 * Description:
 * Send the required string through UART to the other UART device.
 */

void UART_sendString(const uint8 *Str);

/*
 * Description:
 * Receive the required string until the '#' symbol through the UART from another UART device. */
void UART_receiveString(uint8 *Str);



#endif /* UART_H_ */
