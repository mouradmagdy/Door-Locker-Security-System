/*
 * dc_motor.h
 *
 *  Created on: Feb 24, 2024
 *      Author: Mourad
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_
#include "std_types.h"

#define DUTY_CYCLE 256


#define INPUT_ONE_PORT_ID PORTB_ID
#define INPUT_ONE_PIN_ID PIN4_ID

#define INPUT_TWO_PORT_ID PORTB_ID
#define INPUT_TWO_PIN_ID PIN5_ID

#define ENABLE_PORT_ID PORTB_ID
#define ENABLE_PIN_ID PIN3_ID


typedef enum{
	STOP,
	CW,
	A_CW,
}DcMotor_State;

void DcMotor_init(void);

void DcMotor_Rotate(DcMotor_State state);



#endif /* DC_MOTOR_H_ */
