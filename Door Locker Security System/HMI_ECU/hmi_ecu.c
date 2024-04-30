/*
 * hmi_ecu.c
 *
 *  Created on: Mar 23, 2024
 *      Author: Mourad
 */

#include"uart.h"
#include"keypad.h"
#include"lcd.h"
#include<avr/io.h>
#include<util/delay.h>
#include"Timer1.h"

#define PASSWORD_LENGTH 5
#define ENTER_BUTTON 13
#define CONTROL_ECU_READY 0x10
#define HMI_ECU_READY 0x11
#define OPEN_GATE 0x15



uint8 step= 1;
uint8 tick1=0;
uint8 tick=0;

/* function to create and confirm the password*/
void create_system_password(){
	/* send ready byte to control_ecu to begin the transmission*/
	UART_sendByte(HMI_ECU_READY);

	LCD_clearScreen();
	uint8 i=0;
	LCD_displayString("Plz enter pass:");
	LCD_moveCursor(1,0);

	while(i<PASSWORD_LENGTH){
		/* Sending the entered key to the control_ecu*/
		UART_sendByte(KEYPAD_getPressedKey());
		/* delay to not take the same button more than once */
		_delay_ms(250);
		LCD_displayString("*");
		i++;
	}
	/* waiting for the enter key*/
	while(KEYPAD_getPressedKey()!=ENTER_BUTTON);
	/* delay to not take the same button more than once*/
	_delay_ms(150);


		LCD_clearScreen();
		i=0;
		LCD_displayString("Plz re-enter the");
		LCD_moveCursor(1,0);
		LCD_displayString("same pass: ");

		/* Sending ready byte again to begin transmission*/
		UART_sendByte(HMI_ECU_READY);

		while(i<PASSWORD_LENGTH){
			UART_sendByte(KEYPAD_getPressedKey());
			/* delay to not take the same button more than once */
			_delay_ms(250);
			LCD_displayString("*");
			i++;
		}
		while(KEYPAD_getPressedKey()!=ENTER_BUTTON);
		_delay_ms(250);

}

/* Function to read the password from the user when trying to open the door or change passwords*/

void read_password(){
	/* array to store the entered characters to send them by UART*/
	char read_password[PASSWORD_LENGTH];
	LCD_clearScreen();
	uint8 i=0;
	LCD_displayString("Plz enter pass:");
	LCD_moveCursor(1,0);
	while(i<PASSWORD_LENGTH){
//	UART_sendByte(KEYPAD_getPressedKey());
		read_password[i]=KEYPAD_getPressedKey();
	/* delay to not take the same button more than once */
	_delay_ms(250);
	LCD_displayString("*");
	i++;
	}
	while(KEYPAD_getPressedKey()!=ENTER_BUTTON);
	_delay_ms(250);

	/* Sending the characters one by one through uart*/
	for(i=0;i<PASSWORD_LENGTH;i++){
		UART_sendByte(read_password[i]);
	}
}

/* function to rotate the motor and open the door, gets executed when password is correct*/
void rotate_motor_open_door(){
	tick++;
	if(tick==15){
			TCNT1=0;
			LCD_clearScreen();
			LCD_displayString("Locking in 3 sec");
	}
	else if(tick==18){
			TCNT1=0;
			LCD_clearScreen();
			LCD_displayString("Door is Locking");
	}
	else if(tick==33){
			Timer1_deInit();

	}
}

/* Function to lock the system when the entered password is wrong more than 3 times*/
void system_locked(void){
	tick1++;
	if(tick1==30){
//		tick=0;
		Timer1_deInit();
	}
}



int main(void){

	UART_ConfigType uart={EIGHT_BIT_MODE,EVEN_PARITY,ONE_STOP_BIT,RATE_THREE};
	Timer1_ConfigType timer1={
			0,
			31250,
			F_CPU_256,
			COMPARE_MODE
	};
	SREG|=(1<<7);
	LCD_init();

	UART_init(&uart);
	uint8 choice,step=1;

	while(1){
		if(step==1){
			create_system_password();
			/* waiting for the ready byte from control_ecu*/
			while(UART_receiveByte()!=CONTROL_ECU_READY);
			/* receive byte from the control_ecu to tell me which step I'm going*/
			step=UART_receiveByte();



		}
		else if(step==2){
			/* sending ready byte to control_ecu*/
			UART_sendByte(HMI_ECU_READY);
			LCD_clearScreen();
			LCD_displayString("+ : Open Door");
			LCD_moveCursor(1,0);
			LCD_displayString("- : Change Pass");
			choice=KEYPAD_getPressedKey();
			_delay_ms(250);
			if(choice=='+'){
				/* sending + choice to uart so that control_ecu send me which step to go*/
				UART_sendByte(choice);
				/* waiting for the ready byte of control_ecu*/
				while(UART_receiveByte()!=CONTROL_ECU_READY);
				/* receive which step to go*/
				step=UART_receiveByte();
			}
			else if(choice=='-'){
				UART_sendByte(choice);
				while(UART_receiveByte()!=CONTROL_ECU_READY);
				step=UART_receiveByte();
			}

		}
		else if(step==3){
			UART_sendByte(HMI_ECU_READY);
			read_password();

			while(UART_receiveByte()!=CONTROL_ECU_READY);
			uint8 receiveByte=UART_receiveByte();
			if(receiveByte==OPEN_GATE){
				LCD_clearScreen();
				Timer1_init(&timer1);
				LCD_displayString("Door is Unlocking");
				Timer1_setCallBack(&rotate_motor_open_door);
				step=6;
			}
			else if(receiveByte==5){
				step=5;
			}

		}
		else if(step==4){
			UART_sendByte(HMI_ECU_READY);
			read_password();

			while(UART_receiveByte()!=CONTROL_ECU_READY);
			step=UART_receiveByte();

		}
		else if(step==5){
			LCD_clearScreen();
			LCD_displayString("ERROR");
			Timer1_init(&timer1);
			Timer1_setCallBack(&system_locked);
			step=6;

		}
		else if(step==6){
			// for wrong password 3 times
			if(tick1==30){
				step=2;
				tick1=0;
			}
			// for opening the door
			if(tick==33){
						step=2;
						tick=0;
				}
		}
	}


}

