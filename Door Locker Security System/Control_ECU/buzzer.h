/*
 * buzzer.h
 *
 *  Created on: Mar 23, 2024
 *      Author: Mourad
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include"std_types.h"

#define BUZZER_PORT PORTA_ID
#define BUZZER_PIN PIN0_ID


void Buzzer_init();

void Buzzer_on(void);

void Buzzer_off(void);



#endif /* BUZZER_H_ */
