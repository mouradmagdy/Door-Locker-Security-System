/*
 * control_ecu.c
 *
 *  Created on: Mar 23, 2024
 *      Author: Mourad
 */

#include "uart.h"
#include"buzzer.h"
#include"dc_motor.h"
#include"twi.h"
#include"external_eeprom.h"
#include<avr/io.h>
#include<util/delay.h>
#include"timer1.h"

#define CONTROL_ECU_READY 0x10
#define PASSWORD_LENGTH 5
#define HMI_ECU_READY 0x11
#define OPEN_GATE 0x15

uint8 tick=0;
uint8 step=1;



uint8 read_password(){
	uint8 i;
	uint8 password_retrival[PASSWORD_LENGTH];
	uint8 val;
	uint8 password_correct=1;
//	while(UART_receiveByte()!=HMI_ECU_READY);
	for(i=0;i<PASSWORD_LENGTH;i++){
		password_retrival[i]=UART_receiveByte();
		}
//	UART_sendByte(CONTROL_ECU_READY);

	for(i=0;i<PASSWORD_LENGTH;i++){
	EEPROM_readByte(0x0310+i,&val);
	if(val!=password_retrival[i]){
	password_correct=0;
	break;
	}
}

	return password_correct;
}

void rotate_motor_open_door(){
	tick++;
	if(tick==15){
		TCNT1=0;
		DcMotor_Rotate(STOP);
	}
	else if(tick==18){
		TCNT1=0;
		DcMotor_Rotate(A_CW);

	}
	else if(tick==33){
		step=2;
		DcMotor_Rotate(STOP);
		tick=0;
		Timer1_deInit();

	}

}

void system_locked(void){
	tick++;
	if(tick==30){
		tick=0;
		step=2;
		Timer1_deInit();
		Buzzer_off();
	}
}



int main(void){
	SREG|=(1<<7);
	uint8 state;
	uint8 state1;
	uint8 num_wrong=0;
	uint8 num_wrong1=0;



	uint8 i=0;
	UART_ConfigType uart={
			EIGHT_BIT_MODE,
			EVEN_PARITY,
			ONE_STOP_BIT,
			RATE_THREE
	};

	TWI_ConfigType twi={10,400000};
	Timer1_ConfigType timer1={
			0,
			31250,
			F_CPU_256,
			COMPARE_MODE
	};
	TWI_init(&twi);
	DcMotor_init();

	UART_init(&uart);

	while(1){

		if(step==1){
			uint8 password_match=1;
			uint8 password[PASSWORD_LENGTH];
			uint8 password_confirmation[PASSWORD_LENGTH];
			while(UART_receiveByte()!=HMI_ECU_READY);

			for(i=0;i<PASSWORD_LENGTH;i++){
				password[i]=UART_receiveByte();
			}


			while(UART_receiveByte()!=HMI_ECU_READY);

			for(i=0;i<PASSWORD_LENGTH;i++){
				password_confirmation[i]=UART_receiveByte();
			}


			for(i=0;i<PASSWORD_LENGTH;i++){
				if(password[i]!=password_confirmation[i]){
					password_match=0;
					break;
				}
			}
			UART_sendByte(CONTROL_ECU_READY);

			if(password_match){
				step=2;
				UART_sendByte(step);
				for(i=0;i<PASSWORD_LENGTH;i++){
					EEPROM_writeByte(0x0310+i,password[i]);
					_delay_ms(10);
					}
			}
			else{
				UART_sendByte(step);
			}
		}

		else if(step==2){
			uint8 choice;
			while(UART_receiveByte()!=HMI_ECU_READY);
			choice=UART_receiveByte();
			if(choice=='+'){
				step=3;
				UART_sendByte(CONTROL_ECU_READY);
				UART_sendByte(step);
			}
			else{
				step=4;
				UART_sendByte(CONTROL_ECU_READY);
				UART_sendByte(step);
			}
		}
		else if(step==3){
			while(UART_receiveByte()!=HMI_ECU_READY);

			state1=read_password();
			UART_sendByte(CONTROL_ECU_READY);
			if(state1){
				num_wrong1=0;
				UART_sendByte(OPEN_GATE);

				Timer1_init(&timer1);
				DcMotor_Rotate(CW);
				Timer1_setCallBack(&rotate_motor_open_door);
				step=6;
			}
			else if(state1==0&&num_wrong1<2){
				num_wrong1++;
				step=3;
				UART_sendByte(step);
			}
			else if(state1==0&&num_wrong1==2){
				num_wrong1=0;
				step=5;
				UART_sendByte(step);

			}

		}
		else if(step==4){

			while(UART_receiveByte()!=HMI_ECU_READY);

			state=read_password();
			UART_sendByte(CONTROL_ECU_READY);

			if(state){
				num_wrong=0;
				step=1;
				UART_sendByte(step);
			}
			else if(state==0&&num_wrong<2){
				num_wrong++;
				step=4;
				UART_sendByte(step);
			}
			else if(state==0&&num_wrong==2){
				num_wrong=0;
				step=5;
				UART_sendByte(step);

			}
		}
		else if(step==5){
			Buzzer_on();
			Timer1_init(&timer1);
			Timer1_setCallBack(&system_locked);
			step=6;


		}
		else if(step==6){
		}
}
}
