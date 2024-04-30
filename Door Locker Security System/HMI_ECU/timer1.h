/*
 * timer1.h
 *
 *  Created on: Mar 23, 2024
 *      Author: Mourad
 */

#ifndef TIMER1_H_
#define TIMER1_H_

#include"std_types.h"

typedef enum{
	F_CPU_NOCLOCK=0,
	F_CPU_CLOCK,
	F_CPU_8,
	F_CPU_64,
	F_CPU_256,
	F_CPU_1024,

}Timer1_Prescaler;

typedef enum{
	NORMAL_MODE=0,
	COMPARE_MODE=4,

}Timer1_Mode;

typedef struct{
	uint16 initial_value;
	uint16 compare_value; // used in compare mode only
	Timer1_Prescaler prescaler;
	Timer1_Mode mode;
}Timer1_ConfigType;

void Timer1_init(const Timer1_ConfigType*Config_Ptr);

void Timer1_deInit(void);

void Timer1_setCallBack(void(*a_ptr)(void));




#endif /* TIMER1_H_ */
