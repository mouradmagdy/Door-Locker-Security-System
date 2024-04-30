/*
 * pwm.h
 *
 *  Created on: Feb 24, 2024
 *      Author: Mourad
 */

#ifndef PWM_H_
#define PWM_H_
#include "std_types.h"
#define ENABLE_PORT_ID PORTB_ID
#define ENABLE_PIN_ID PIN3_ID

void PWM_TIMER0_start(uint8 duty_cycle);



#endif /* PWM_H_ */
