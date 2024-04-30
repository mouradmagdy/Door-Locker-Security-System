/*
 * dc_motor.c
 *
 *  Created on: Feb 24, 2024
 *      Author: Mourad
 */

#include"dc_motor.h"
#include"gpio.h"
#include"common_macros.h"
#include<avr/io.h>
#include"pwm.h"

void DcMotor_init(void){
	/* set the pins of the motor as output*/
	GPIO_setupPinDirection(INPUT_ONE_PORT_ID,INPUT_ONE_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(INPUT_TWO_PORT_ID,INPUT_TWO_PIN_ID,PIN_OUTPUT);

	/* the motor is initially stopped*/
	GPIO_writePin(INPUT_ONE_PORT_ID,INPUT_ONE_PIN_ID,LOGIC_LOW);
	GPIO_writePin(INPUT_TWO_PORT_ID,INPUT_TWO_PIN_ID,LOGIC_LOW);


}


void DcMotor_Rotate(DcMotor_State state){
	uint8 speed=100;
	speed=(speed*(DUTY_CYCLE-1))/100;


	PWM_TIMER0_start(speed);

	switch (state){
	case STOP:
		GPIO_writePin(INPUT_ONE_PORT_ID,INPUT_ONE_PIN_ID,LOGIC_LOW);
		GPIO_writePin(INPUT_TWO_PORT_ID,INPUT_TWO_PIN_ID,LOGIC_LOW);
		break;
	case CW:
		GPIO_writePin(INPUT_TWO_PORT_ID,INPUT_TWO_PIN_ID,LOGIC_HIGH);
		GPIO_writePin(INPUT_ONE_PORT_ID,INPUT_ONE_PIN_ID,LOGIC_LOW);
		break;
	case A_CW:
		GPIO_writePin(INPUT_TWO_PORT_ID,INPUT_TWO_PIN_ID,LOGIC_LOW);
		GPIO_writePin(INPUT_ONE_PORT_ID,INPUT_ONE_PIN_ID,LOGIC_HIGH);
		break;





	}
}
