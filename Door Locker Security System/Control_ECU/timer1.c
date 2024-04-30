/*
 * timer1.c
 *
 *  Created on: Mar 23, 2024
 *      Author: Mourad
 */
#include"timer1.h"
#include<avr/io.h>
#include<avr/interrupt.h>

volatile void(*callback_ptr)(void)=((void*)0);
ISR(TIMER1_COMPA_vect){
	if(callback_ptr!=((void*)0)){
			(*callback_ptr)();
		}
}

ISR(TIMER1_OVF_vect){
	if(callback_ptr!=((void*)0)){
		(*callback_ptr)();
	}
}

void Timer1_init(const Timer1_ConfigType*Config_Ptr){
	/* Initial value of the timer*/
	TCNT1=Config_Ptr->initial_value;

	TCCR1A=(1<<FOC0);
	/* Adjusting the prescaler of the timer while preserving the other bits*/
	TCCR1B=(TCCR1B&0xF8)|(Config_Ptr->prescaler&0x07);
	TCCR1B=(TCCR1B&0xF7)|(Config_Ptr->mode&0x08);

	if(Config_Ptr->mode==4){
		OCR1A=Config_Ptr->compare_value;
		TIMSK|=(1<<OCIE1A);
	}
	else if(Config_Ptr->mode==0){
		TIMSK|=(1<<TOIE1);
	}

}

void Timer1_deInit(void){
	TCNT1=0;
	TCCR1A=0;
	TCCR1B=0;
	OCR1A=0;
	TIMSK=0;

}

void Timer1_setCallBack(void(*a_ptr)(void)){
	callback_ptr=a_ptr;
}

