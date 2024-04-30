/*
 * pwm.c
 *
 *  Created on: Feb 24, 2024
 *      Author: Mourad
 */

#include"gpio.h"
#include<avr/io.h>
#include"pwm.h"

void PWM_TIMER0_start(uint8 duty_cycle){
		OCR0  = duty_cycle;

		/* set the enable pin of the motor as output pin*/
		GPIO_setupPinDirection(ENABLE_PORT_ID,ENABLE_PIN_ID,PIN_OUTPUT);

		TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);
}

